<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Test\Mock;

use Zynga\Framework\IO\Disk\V1\Manager\UNIX as BaseManager;

class ManagerWithFailedToCloseFile extends BaseManager {

  <<__Override>>
  public function checkOrCreatePath(string $path, int $permissions): bool {
    return true;
  }

  <<__Override>>
  public function fileOpen(string $path, string $mode): mixed {
    return tmpfile();
  }

  <<__Override>>
  public function fwrite(resource $handle, string $dataToWrite): mixed {
    return true;
  }

  <<__Override>>
  public function fclose(resource $handle): bool {
    return false;
  }

}
