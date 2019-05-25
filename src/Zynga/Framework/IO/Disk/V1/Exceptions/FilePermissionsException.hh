<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Exceptions;

use Zynga\Framework\IO\Disk\V1\Exceptions\DiskIOException;

/**
 * Represents that file permissions did not allow an operation
 */
abstract class FilePermissionsException extends DiskIOException {}
