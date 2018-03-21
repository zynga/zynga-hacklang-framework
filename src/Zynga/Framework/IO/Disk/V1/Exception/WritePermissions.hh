<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Exception;

use Zynga\Framework\IO\Disk\V1\Exception\FilePermissions as FilePermissionsException;

/**
 * Represents that a file did not the correct permissions to be written to
 */
class WritePermissions extends FilePermissionsException {}
