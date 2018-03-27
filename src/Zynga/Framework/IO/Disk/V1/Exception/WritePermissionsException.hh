<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Exception;

use Zynga\Framework\IO\Disk\V1\Exception\FilePermissionsException;

/**
 * Represents that a file did not have the correct permissions to be written to
 */
class WritePermissionsException extends FilePermissionsException {}
