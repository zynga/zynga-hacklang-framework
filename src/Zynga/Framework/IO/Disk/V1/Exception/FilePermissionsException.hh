<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Exception;

use Zynga\Framework\IO\Disk\V1\Exception\DiskIOException;

/**
 * Represents that file permissions did not allow an operation
 */
abstract class FilePermissionsException extends DiskIOException {}
