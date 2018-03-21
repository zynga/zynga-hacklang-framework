<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Exception;

use Zynga\Framework\IO\Disk\V1\Exception\DiskIO as DiskIOException;

/**
 * Represents that file permissions did not allow an operation
 */
abstract class FilePermissions extends DiskIOException {}
