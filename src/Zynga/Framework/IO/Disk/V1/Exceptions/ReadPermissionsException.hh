<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Exceptions;

use Zynga\Framework\IO\Disk\V1\Exceptions\FilePermissionsException;

/**
 * Represents that a file did not have the correct permissions to be read from
 */
class ReadPermissionsException extends FilePermissionsException {}
