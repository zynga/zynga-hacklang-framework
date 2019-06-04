<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Test\Mock;

use Zynga\Framework\IO\Disk\V1\Manager\UNIX as BaseManager;

class ManagerWithFailedReadPermissions extends BaseManager {

  <<__Override>>
  public function doesFileExist(string $fileName): bool {
    return true;
  }

  <<__Override>>
  public function isReadable(string $path): bool {
    return false;
  }

}
