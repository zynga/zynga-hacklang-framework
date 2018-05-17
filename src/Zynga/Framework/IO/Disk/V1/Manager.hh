<?hh // strict

namespace Zynga\Framework\IO\Disk\V1;

use \Exception;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToCloseFileException;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToCreateDirectoryException;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToDeleteFileException;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToOpenFileException;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToWriteToFileException;
use Zynga\Framework\IO\Disk\V1\Exception\InvalidFileNameException;
use Zynga\Framework\IO\Disk\V1\Exception\ReadPermissionsException;
use Zynga\Framework\IO\Disk\V1\Exception\WritePermissionsException;
use Zynga\Framework\IO\Disk\V1\ManagerInterface as DiskIOManagerInterface;

/**
 * Lightweight class for managing Disk IO
 */
class Manager implements DiskIOManagerInterface {

  private static ?DiskIOManagerInterface $instance;

  public static function instance(): DiskIOManagerInterface {
    if (self::$instance === null) {
      self::$instance = new self();
    }
    invariant(self::$instance !== null, '$instance was null');

    return self::$instance;
  }

  protected function __construct() {}

  /**
   * See @ManagerInterface
   */
  public function checkOrCreatePath(string $path, int $permissions): bool {
    if (!$this->doesFileExist($path)) {
      return $this->makeDirectory($path, $permissions, true);
    }

    return true;
  }

  /**
   * @see ManagerInterface
   */
  public function deleteFile(string $path): bool {
    if ($this->doesFileExist($path)) {
      return $this->unlink($path);
    }

    return true;
  }

  /**
   * @see ManagerInterface
   */
  public function deleteDirectory(string $path): void {
    try {
      if (!$this->rmdir($path)) {
        throw new Exception("Failed to delete directory='$path'");
      }
    } catch (Exception $e) {
      // Not only might we throw our own exception above, but
      // rmdir can itself throw an exception.
      throw new FailedToDeleteFileException($e->getMessage());
    }
  }

  /**
   * @see ManagerInterface
   */
  public function recursivelyDeleteDirectory(
    string $path,
    int $minimumMillisecondsSinceModificaiton = 0,
  ): int {
    $totalFilesDeleted = 0;

    if ($path == '/' ||
        $path == 'c:\\' ||
        !$this->doesFileExist($path) ||
        !$this->isDirectory($path)) {
      return $totalFilesDeleted;
    }

    $resources = $this->scanDirectory($path);
    foreach ($resources as $resource) {
      if ($resource != "." && $resource != "..") {
        if ($this->isDirectory($path."/".$resource)) {
          $totalFilesDeleted += $this->recursivelyDeleteDirectory(
            $path."/".$resource,
            $minimumMillisecondsSinceModificaiton,
          );
        } else if ((time() - filemtime($path."/".$resource)) >=
                   $minimumMillisecondsSinceModificaiton) {
          $totalFilesDeleted +=
            $this->deleteFile($path."/".$resource) ? 1 : 0;
        }
      }
    }

    try {
      if ((time() - filemtime($path)) >= $minimumMillisecondsSinceModificaiton &&
          $this->scanDirectory($path)->count() <= 2) {
        $this->deleteDirectory($path);
        ++$totalFilesDeleted;
      }
    } catch (FailedToDeleteFileException $e) {
    }

    return $totalFilesDeleted;
  }

  /**
   * @see ManagerInterface
   */
  public function writeFile(
    string $fileName,
    string $dataToWrite,
    int $permissions,
    bool $appendIfExists,
  ): void {

    $filePath = $this->directoryName($fileName);
    if ($this->checkOrCreatePath($filePath, $permissions) === false) {
      throw new FailedToCreateDirectoryException($filePath);
    }

    $mode = 'w';
    if ($appendIfExists) {
      $mode = 'a';
    }

    $handle = $this->fileOpen($fileName, $mode);
    if ($handle === false) {
      throw new FailedToOpenFileException($fileName);
    }

    invariant(is_resource($handle), '');
    $writeResult = $this->fwrite($handle, $dataToWrite);
    if ($writeResult === false) {
      $this->fclose($handle);
      throw new FailedToWriteToFileException($fileName);
    }

    if ($writeResult != strlen($dataToWrite) &&
        // fwrite always writes at least one byte if successful, even on
        // empty string input
        !($writeResult == 1 && strlen($dataToWrite) === 0)) {
      $this->fclose($handle);
      throw new FailedToWriteToFileException(
        "Only wrote ".
        (string) $writeResult.
        " of ".
        strlen($dataToWrite).
        " bytes to $fileName",
      );
    }

    $closeResult = $this->fclose($handle);
    if ($closeResult === false) {
      throw new FailedToCloseFileException($fileName);
    }
  }

  /**
   * @see ManagerInterface
   */
  public function tarball(string $in, string $out): void {
    if (!$this->doesFileExist($in) || !$this->isReadable($in)) {
      throw new ReadPermissionsException($in);
    }

    if ((!$this->doesFileExist($out) && !$this->isWriteable(
           $this->directoryName($out),
         ) ||
         ($this->doesFileExist($out) && !$this->isWriteable($out)))) {
      throw new WritePermissionsException($out);
    }

    if (strpos($in, '\'') !== false || strpos($out, '\'') !== false) {
      throw new InvalidFileNameException("in=$in, out=$out");
    }

    if (file_exists($out)) {
      throw new InvalidFileNameException($out);
    }

    $inDir = $this->directoryName($in);
    $trimmedIn = ltrim(substr($in, strlen($inDir)), '/');
    $cmd = 'tar -C';
    $cmd .= ' ';
    $cmd .= escapeshellarg($inDir);
    $cmd .= ' ';
    $cmd .= '-cf';
    $cmd .= ' ';
    $cmd .= escapeshellarg($out);
    $cmd .= ' ';
    $cmd .= escapeshellarg($trimmedIn);
    $cmd .= ' ';
    $cmd .= '2>/dev/null';

    $returnCode = 0;
    $results = array();
    exec($cmd, $results, $returnCode);

    if ($returnCode !== 0 ||
        count($results) !== 0 ||
        !file_exists($out) ||
        !$this->tarbalValid($out)) {
      throw new FailedToWriteToFileException($out);
    }
  }

  /**
   * @see ManagerInterface
   */
  public function bzip2(string $in, string $out): void {
    if (!$this->doesFileExist($in) || !$this->isReadable($in)) {
      throw new ReadPermissionsException($in);
    }

    if ((!$this->doesFileExist($out) && !$this->isWriteable(
           $this->directoryName($out),
         ) ||
         ($this->doesFileExist($out) && !$this->isWriteable($out)))) {
      throw new WritePermissionsException($out);
    }

    $inFile = $this->fileOpen($in, "r");
    if ($inFile === false) {
      throw new FailedToOpenFileException($in);
    }
    invariant(is_resource($inFile), '');

    $outFile = $this->bzopen($out, "w");
    if ($outFile === false) {
      $this->fclose($inFile);
      throw new FailedToOpenFileException($out);
    }
    invariant(is_resource($outFile), '');

    while (!$this->feof($inFile)) {
      $buffer = $this->fgets($inFile, 4096);
      $this->bzwrite($outFile, $buffer, 4096);
    }

    if (!$this->fclose($inFile)) {
      $this->bzclose($outFile);
      throw new FailedToCloseFileException($in);
    }

    if (!$this->bzclose($outFile)) {
      throw new FailedToCloseFileException($out);
    }
  }

  /**
   * @see ManagerInterface
   */
  public function chown(string $fileName, string $userName): bool {
    try {
      if (posix_getpwuid(fileowner($fileName))['name'] == $userName) {
        return true;
      }

      return chown($fileName, $userName);
    } catch (Exception $e) {
    }

    return false;
  }

  protected function isDirectory(string $path): bool {
    return is_dir($path);
  }

  protected function feof(resource $handle): bool {
    return feof($handle);
  }

  protected function fgets(resource $handle, int $maxBytesToRead): string {
    $result = fgets($handle, $maxBytesToRead);
    if (is_string($result)) {
      return $result;
    }

    return '';
  }

  protected function doesFileExist(string $fileName): bool {
    return file_exists($fileName);
  }

  protected function fileOpen(string $fileName, string $mode): mixed {
    try {
      return fopen($fileName, $mode);
    } catch (Exception $e) {
      return false;
    }
  }

  protected function directoryName(string $fileName): string {
    return dirname($fileName);
  }

  protected function makeDirectory(
    string $path,
    int $permissions,
    bool $recursivePermissions,
  ): bool {
    $old = umask(0);
    try {
      return mkdir($path, $permissions, $recursivePermissions);
      umask($old);
    } catch (Exception $e) {
      umask($old);
      return false;
    }
  }

  protected function unlink(string $fileName): bool {
    try {
      return unlink($fileName);
    } catch (Exception $e) {
      return false;
    }
  }

  protected function fwrite(resource $handle, string $dataToWrite): mixed {
    return fwrite($handle, $dataToWrite);
  }

  protected function fclose(resource $handle): bool {
    return fclose($handle);
  }

  protected function isReadable(string $fileName): bool {
    return is_readable($fileName);
  }

  protected function isWriteable(string $fileName): bool {
    return is_writeable($fileName);
  }

  protected function bzopen(string $fileName, string $mode): mixed {
    try {
      return bzopen($fileName, $mode);
    } catch (Exception $e) {
      return false;
    }
  }

  protected function bzclose(resource $handle): bool {
    return bzclose($handle);
  }

  protected function bzwrite(
    resource $handle,
    string $dataToWrite,
    int $maxBytesToRead,
  ): int {
    $result = bzwrite($handle, $dataToWrite, $maxBytesToRead);
    if (is_int($result)) {
      return $result;
    }

    return 0;
  }

  /**
   * @throws Exception
   */
  protected function rmdir(string $path): bool {
    return rmdir($path);
  }

  protected function scanDirectory(string $path): Vector<resource> {
    $results = Vector {};

    $objects = scandir($path);
    if ($objects === false) {
      return $results;
    }
    $results->addAll($objects);
    return $results;
  }

  protected function tarbalValid(string $tarPath): bool {
    $output = array();
    if (!$this->doesFileExist($tarPath)) {
      return false;
    }

    $cmd = 'tar -df';
    $cmd .= ' ';
    $cmd .= escapeshellarg($tarPath);
    $cmd .= ' ';
    $cmd .= '2>/dev/null';
    exec($cmd, $output);
    return count($output) === 0;
  }
}
