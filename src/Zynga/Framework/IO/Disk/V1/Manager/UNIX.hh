<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Manager;

use Zynga\Framework\IO\Disk\V1\Manager\Base;
use \Exception;

class UNIX extends Base {

  public function isDirectory(string $path): bool {
    return is_dir($path);
  }

  public function feof(resource $handle): bool {
    return feof($handle);
  }

  public function fgets(resource $handle, int $maxBytesToRead): string {

    $result = fgets($handle, $maxBytesToRead);

    if (is_string($result)) {
      return $result;
    }

    return '';

  }

  public function doesFileExist(string $fileName): bool {
    return file_exists($fileName);
  }

  public function fileOpen(string $fileName, string $mode): mixed {
    try {
      return fopen($fileName, $mode);
    } catch (Exception $e) {
      return false;
    }
  }

  public function directoryName(string $fileName): string {
    return dirname($fileName);
  }

  public function makeDirectory(
    string $path,
    int $permissions,
    bool $recursivePermissions,
  ): bool {
    $old = umask(0);
    try {
      $rv = mkdir($path, $permissions, $recursivePermissions);
      umask($old);
      return $rv;
    } catch (Exception $e) {
      umask($old);
      return false;
    }
  }

  public function unlink(string $fileName): bool {
    try {
      return unlink($fileName);
    } catch (Exception $e) {
      return false;
    }
  }

  public function fwrite(resource $handle, string $dataToWrite): mixed {
    return fwrite($handle, $dataToWrite);
  }

  public function fclose(resource $handle): bool {
    return fclose($handle);
  }

  public function isReadable(string $fileName): bool {
    return is_readable($fileName);
  }

  public function isWriteable(string $fileName): bool {
    return is_writeable($fileName);
  }

  public function bzopen(string $fileName, string $mode): mixed {
    try {
      return bzopen($fileName, $mode);
    } catch (Exception $e) {
      return false;
    }
  }

  public function bzclose(resource $handle): bool {
    return bzclose($handle);
  }

  /**
   * @throws Exception
   */
  public function rmdir(string $path): bool {
    return rmdir($path);
  }

  public function bzwrite(
    resource $handle,
    string $dataToWrite,
    int $maxBytesToRead,
  ): int {
    // --
    // Emits to stdout when given invalid data to work with.
    // --
    $result = @bzwrite($handle, $dataToWrite, $maxBytesToRead);
    if (is_int($result)) {
      return $result;
    }

    return 0;
  }

  public function scanDirectory(string $path): Vector<resource> {
    $results = Vector {};

    // --
    // Emits to stdout when given a invalid or unreachable path.
    // --
    $objects = @scandir($path);

    if ($objects === false) {
      return $results;
    }

    $results->addAll($objects);

    return $results;

  }

  public function isTarballValid(string $tarPath): bool {
    $output = array();
    if (!$this->doesFileExist($tarPath)) {
      return false;
    }

    // TODO: Tar finder.
    $cmd = 'tar -df';
    $cmd .= ' ';
    $cmd .= escapeshellarg($tarPath);
    $cmd .= ' ';
    $cmd .= '2>/dev/null';
    exec($cmd, $output);
    return count($output) === 0;
  }

}
