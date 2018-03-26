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
   * See @ManagerInterface
   */
  public function deleteFile(string $path): bool {
    if ($this->doesFileExist($path)) {
      return $this->unlink($path);
    }

    return true;
  }

  /**
   * See @ManagerInterface
   */
  public function writeFile(string $fileName, string $dataToWrite, int $permissions): void {
    $filePath = $this->directoryName($fileName);
    if ($this->checkOrCreatePath($filePath, $permissions) === false) {
      throw new FailedToCreateDirectoryException($filePath);
    }

    $handle = $this->fileOpen($fileName, 'w');
    if ($handle === false) {
      throw new FailedToOpenFileException($fileName);
    }

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
   * See @ManagerInterface
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

    $outFile = $this->bzopen($out, "w");
    if ($outFile === false) {
      $this->fclose($inFile);
      throw new FailedToOpenFileException($out);
    }

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

  // Have to use mixed for $handle here because Hack doesn't allow
  // passing resource types to functions
  protected function feof(mixed $handle): bool {
    if (is_resource($handle)) {
      return feof($handle);
    }

    return false;
  }

  // Have to use mixed for $handle here because Hack doesn't allow
  // passing resource types to functions
  protected function fgets(mixed $handle, int $maxBytesToRead): string {
    if (is_resource($handle)) {
      $result = fgets($handle, $maxBytesToRead);
      if (is_string($result)) {
        return $result;
      }
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

  // Have to use mixed for $handle here because Hack doesn't allow
  // passing resource types to functions
  protected function fwrite(mixed $handle, string $dataToWrite): mixed {
    if (is_resource($handle)) {
      return fwrite($handle, $dataToWrite);
    }

    return false;
  }

  // Have to use mixed for $handle here because Hack doesn't allow
  // passing resource types to functions
  protected function fclose(mixed $handle): bool {
    if (is_resource($handle)) {
      return fclose($handle);
    }

    return false;
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

  // Have to use mixed for $handle here because Hack doesn't allow
  // passing resource types to functions
  protected function bzclose(mixed $handle): bool {
    if (is_resource($handle)) {
      return bzclose($handle);
    }

    return false;
  }

  // Have to use mixed for $handle here because Hack doesn't allow
  // passing resource types to functions
  protected function bzwrite(mixed $handle, string $dataToWrite, int $maxBytesToRead): int {
    if (is_resource($handle)) {
      $result = bzwrite($handle, $dataToWrite, $maxBytesToRead);
      if (is_int($result)) {
        return $result;
      }
    }

    return 0;
  }
}
