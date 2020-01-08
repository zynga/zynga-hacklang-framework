<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Test\Mock;

use Zynga\Framework\IO\Disk\V1\Manager\UNIX as BaseManager;

class ManagerWithRmdirFalse extends BaseManager {

  <<__Override>>
  public function rmdir(string $path): bool {
    return false;
  }

}
