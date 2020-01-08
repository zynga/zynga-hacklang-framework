<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Exceptions;

use Zynga\Framework\IO\Disk\V1\Exceptions\FilePermissionsException;

/**
 * Represents that an invalid file name was attempted to be used
 */
class InvalidFileNameException extends FilePermissionsException {}
