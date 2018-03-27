<?hh // strict

namespace Zynga\Framework\IO\Disk\V1;

use \Exception;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToCloseFileException;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToCreateDirectoryException;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToOpenFileException;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToWriteToFileException;
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

  protected function __construct() {
  }

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
  public function writeFile(string $fileName, string $dataToWrite, int $permissions, bool $appendIfExists): void {
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

    invariant(is_resource($handle),'');
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
        "Only wrote ".(string)$writeResult." of ".strlen($dataToWrite)." bytes to $fileName"
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
  public function bzip2(string $in, string $out): void {
    if (!$this->doesFileExist($in) || !$this->isReadable($in)) {
      throw new ReadPermissionsException($in);
    }

    if ((!$this->doesFileExist($out) && !$this->isWriteable($this->directoryName($out)) ||
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

  protected function makeDirectory(string $path, int $permissions, bool $recursivePermissions): bool {
    try {
      return mkdir($path, $permissions, $recursivePermissions);
    } catch (Exception $e) {
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

  protected function bzwrite(resource $handle, string $dataToWrite, int $maxBytesToRead): int {
    $result = bzwrite($handle, $dataToWrite, $maxBytesToRead);
    if (is_int($result)) {
      return $result;
    }

    return 0;
  }
}
