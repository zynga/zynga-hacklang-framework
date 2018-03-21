<?hh // strict

namespace Zynga\Framework\IO\Disk\V1;

use Zynga\Framework\IO\Disk\V1\Exception\FailedToCloseFile as FailedToCloseFileException;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToCreateDirectory as FailedToCreateDirectoryException;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToOpenFile as FailedToOpenFileException;
use Zynga\Framework\IO\Disk\V1\Exception\FailedToWriteToFile as FailedToWriteToFileException;
use Zynga\Framework\IO\Disk\V1\Exception\ReadPermissions as ReadPermissionsException;
use Zynga\Framework\IO\Disk\V1\Exception\WritePermissions as WritePermissionsException;

/**
 * Lightweight class for managing Disk IO
 */
class Manager {

  /**
   * Creates a folder at the given path if none exists.
   *
   * @param $path Absolute path of directory to create
   * @param $permissions Integer flag rpresenting file permissions
   * @return bool True if the path already exists or was successfully created,
   *         otherwise false
   *
   */
  public static function checkOrCreatePath(string $path, int $permissions): bool {
    if (!file_exists($path)) {
      return mkdir($path, $permissions, true);
    }

    return true;
  }

  /**
   * Deletes the file at the given path if one exists.
   *
   * @param $path Absolute path of directory to create
   * @return bool True if the file does not exist or was successfully deleted,
   *         otherwise false
   */
  public static function deleteFile(string $path): bool {
    if (file_exists($path)) {
      return unlink($path);
    }

    return true;
  }

  /**
   * Creates (if neccessary) a file at the given path and writes the provided
   * data to it.
   *
   * @param $fileName Absolute path of file to create/write to
   * @param $dataToWrite Data to write to file
   * @throws FailedToCreateDirectoryException
   * @throws FailedToOpenFileException
   * @throws FailedToWriteToFileException
   * @throws FailedToCloseFileException
   */
  public static function writeFile(string $fileName, string $dataToWrite): void {
    $filePath = dirname($fileName);
    if (!$this->checkOrCreatePath($filePath)) {
      throw new FailedToCreateDirectoryException($filePath);
    }

    $handle = fopen($fileName, 'w');
    if ($handle === false) {
      throw new FailedToOpenFileException($fileName);
    }

    $writeResult = fwrite($handle, $dataToWrite);

    if ($writeResult === false) {
      fclose($handle);
      throw new FailedToWriteToFileException($fileName);
    }

    if ($writeResult != strlen($dataToWrite)) {
      fclose($handle);
      throw new FailedToWriteToFileException(
        "Only wrote $writeResult of ".strlen($dataToWrite)." bytes to $fileName"
      );
    }

    $closeResult = fclose($handle);
    if ($closeResult === false) {
      throw new FailedToCloseFileException($fileName);
    }
  }

  /**
   * Compresses the given file into a bzip2 file at the given output path.
   *
   * @param $in Absolute path of file to compress
   * @param $out Absolute path of output bzip2 file
   * @throws ReadPermissionsException
   * @throws WritePermissionsException
   * @throws FailedToOpenFileException
   * @throws FailedToCloseFileException
   */
  public static function bzip2(string $in, string $out): void {
    if (!file_exists($in) || !is_readable($in)) {
      throw new ReadPermissionsException($in);
    }

    if ((!file_exists($out) && !is_writeable(dirname($out)) ||
        (file_exists($out) && !is_writable($out)))) {
      throw new WritePermissionsException($out);
    }

    $inFile = fopen($in, "r");
    if ($inFile === false) {
      throw new FailedToOpenFileException($in);
    }

    $outFile = bzopen($out, "w");
    if ($outFile === false) {
      fclose($inFile);
      throw new FailedToOpenFileException($out);
    }

    while (!feof($inFile)) {
      $buffer = fgets($inFile, 4096);
      bzwrite($outFile, $buffer, 4096);
    }

    if (!fclose($inFile)) {
      bzclose($outFile);
      throw new FailedToCloseFileException($in);
    }

    if (!bzclose($outFile)) {
      throw new FailedToCloseFileException($out);
    }
  }
}
