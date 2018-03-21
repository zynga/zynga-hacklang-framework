<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Exception;

use Zynga\Framework\IO\V1\Exception\IO as IOException;

/**
 * Base Disk Input/Output exception from which all others derive.
 * Represents that there was an exception with a disk input or
 * output operation
 */
abstract class DiskIO extends IOException {}
