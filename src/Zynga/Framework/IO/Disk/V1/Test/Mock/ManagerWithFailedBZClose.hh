<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Test\Mock;

use Zynga\Framework\IO\Disk\V1\Manager\UNIX as BaseManager;

class ManagerWithFailedBZClose extends BaseManager {

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
  public function fileOpen(string $path, string $mode): mixed {
    return tmpfile();
  }

  <<__Override>>
  public function bzopen(string $path, string $mode): mixed {
    return tmpfile();
  }

  <<__Override>>
  public function bzclose(resource $handle): bool {
    return false;
  }

}
