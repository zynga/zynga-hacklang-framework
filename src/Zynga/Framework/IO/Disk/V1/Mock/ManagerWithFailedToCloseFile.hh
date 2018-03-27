<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Mock;

use Zynga\Framework\IO\Disk\V1\Manager as BaseManager;
use Zynga\Framework\IO\Disk\V1\ManagerInterface as DiskIOManagerInterface;

class ManagerWithFailedToCloseFile extends BaseManager {

  private static ?DiskIOManagerInterface $instance;

  <<__Override>>
  public static function instance(): DiskIOManagerInterface {
    if (self::$instance === null) {
      self::$instance = new self();
    }
    invariant(self::$instance !== null, '$instance was null');

    return self::$instance;
  }

  <<__Override>>
  public function checkOrCreatePath(string $path, int $permissions): bool {
    return true;
  }

  <<__Override>>
  protected function fileOpen(string $path, string $mode): mixed {
    return tmpfile();
  }

  <<__Override>>
  protected function fwrite(resource $handle, string $dataToWrite): mixed {
    return true;
  }

  <<__Override>>
  protected function fclose(resource $handle): bool {
    return false;
  }

}
