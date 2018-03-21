<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Exception;

use Zynga\Framework\IO\Disk\V1\Exception\DiskIO as DiskIOException;

/**
 * Represents that an operation to close a file failed
 */
class FailedToCloseFile extends DiskIOException {}
