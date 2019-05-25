<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Exceptions;

use Zynga\Framework\IO\Disk\V1\Exceptions\FilePermissionsException;

/**
 * Represents that a file did not have the correct permissions to be written to
 */
class WritePermissionsException extends FilePermissionsException {}
