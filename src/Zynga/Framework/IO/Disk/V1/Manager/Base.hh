<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Manager;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\IO\Disk\V1\Interfaces\ManagerInterface;
use Zynga\Framework\IO\Disk\V1\Exceptions\FailedToCloseFileException;
use Zynga\Framework\IO\Disk\V1\Exceptions\FailedToCreateDirectoryException;
use Zynga\Framework\IO\Disk\V1\Exceptions\FailedToDeleteDirectoryException;
use Zynga\Framework\IO\Disk\V1\Exceptions\FailedToDeleteFileException;
use Zynga\Framework\IO\Disk\V1\Exceptions\FailedToOpenFileException;
use Zynga\Framework\IO\Disk\V1\Exceptions\FailedToWriteToFileException;
use Zynga\Framework\IO\Disk\V1\Exceptions\InvalidFileNameException;
use Zynga\Framework\IO\Disk\V1\Exceptions\ReadPermissionsException;
use Zynga\Framework\IO\Disk\V1\Exceptions\WritePermissionsException;

abstract class Base implements ManagerInterface {

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
        throw new FailedToDeleteDirectoryException(
          "Failed to delete directory='$path'",
        );
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
    int $minimumSecondsSinceModification = 0,
  ): int {
    $totalFilesDeleted = 0;

    // --
    // TODO: Move this saftey check into the individual drivers, and
    //       split it away frmo the existance check.
    // --
    if ($path == '/' ||
        $path == 'c:\\' ||
        !$this->doesFileExist($path) ||
        !$this->isDirectory($path)) {
      return $totalFilesDeleted;
    }

    $resources = $this->scanDirectory($path);
    foreach ($resources as $resource) {
      // JEO: This is okay as the . and .. are transliterated within php internals.
      if ($resource != "." && $resource != "..") {
        // TODO: fix the hardcoded unix slash.
        if ($this->isDirectory($path."/".$resource)) {
          $totalFilesDeleted += $this->recursivelyDeleteDirectory(
            $path."/".$resource,
            $minimumSecondsSinceModification,
          );
          // TODO: fix the hardcoded unix slash
        } else if ((time() - filemtime($path."/".$resource)) >=
                   $minimumSecondsSinceModification) {
          $totalFilesDeleted += // TODO: fix the hardcoded unix slash
 $this->deleteFile($path."/".$resource) ? 1 : 0;
        }
      }
    }

    try {
      if ((time() - filemtime($path)) >= $minimumSecondsSinceModification &&
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

    // TODO: move this to do a simpler is_resource check on fh open.
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

    // TODO: implement the command finder pattern.
    $inDir = $this->directoryName($in);

    // TODO: fix the path / assumption.
    $trimmedIn = ltrim(substr($in, strlen($inDir)), '/');

    // TODO: is this tar command specific to a particular unix?
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
        !$this->isTarballValid($out)) {
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

    // TODO: evaluate conversion to simpler is_resource check.
    $inFile = $this->fileOpen($in, "r");
    if ($inFile === false) {
      throw new FailedToOpenFileException($in);
    }
    invariant(is_resource($inFile), '');

    // TODO: evaluate conversion to a simpler is_resource check.
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

    // TODO: Clearly a unix choice here, as posix is not avail on windows.
    if (posix_getpwuid(fileowner($fileName))['name'] == $userName) {
      return true;
    }

    // --
    // JEO: This is an internal that emits output, we have to supress.
    // --
    return @chown($fileName, $userName);

  }

}
