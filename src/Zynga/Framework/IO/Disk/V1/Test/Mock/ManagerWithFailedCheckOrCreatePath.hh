<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Test\Mock;

use Zynga\Framework\IO\Disk\V1\Manager\UNIX as BaseManager;

class ManagerWithFailedCheckOrCreatePath extends BaseManager {

  <<__Override>>
  public function checkOrCreatePath(string $path, int $permissions): bool {
    return false;
  }

}
