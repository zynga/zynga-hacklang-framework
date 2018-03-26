<?hh // strict

namespace Zynga\Framework\IO\Disk\V1;

/**
 * Interface for managing Disk IO
 */
interface ManagerInterface {

  /**
   * Creates a folder at the given path if none exists.
   *
   * @param $path Absolute path of directory to create
   * @param $permissions Integer flag representing file permissions
   * @return bool True if the path already exists or was successfully created,
   *         otherwise false
   *
   */
  public function checkOrCreatePath(string $path, int $permissions): bool;

  /**
   * Deletes the file at the given path if one exists.
   *
   * @param $path Absolute path of directory to create
   * @return bool True if the file does not exist or was successfully deleted,
   *         otherwise false
   */
  public function deleteFile(string $path): bool;

  /**
   * Creates (if neccessary) a file at the given path and writes the provided
   * data to it.
   *
   * @param $fileName Absolute path of file to create/write to
   * @param $dataToWrite Data to write to file
   * @param $permissions Integer flag rpresenting file permissions
   * @throws FailedToCreateDirectoryException
   * @throws FailedToOpenFileException
   * @throws FailedToWriteToFileException
   * @throws FailedToCloseFileException
   */
  public function writeFile(string $fileName, string $dataToWrite, int $permissions): void;

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
  public function bzip2(string $in, string $out): void;
}
