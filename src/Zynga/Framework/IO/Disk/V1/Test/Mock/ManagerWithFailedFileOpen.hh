<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Test\Mock;

use Zynga\Framework\IO\Disk\V1\Manager\UNIX as BaseManager;

class ManagerWithFailedFileOpen extends BaseManager {

  <<__Override>>
  public function doesFileExist(string $fileName): bool {
    return true;
  }

  <<__Override>>
  public function isReadable(string $path): bool {
    return true;
  }

  <<__Override>>
  public function isWriteable(string $path): bool {
    return true;
  }

  <<__Override>>
  public function checkOrCreatePath(string $path, int $permissions): bool {
    return true;
  }

  <<__Override>>
  public function fileOpen(string $path, string $mode): mixed {
    return false;
  }

}
