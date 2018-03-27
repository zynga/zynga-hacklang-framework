<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Mock;

use Zynga\Framework\IO\Disk\V1\Manager as BaseManager;
use Zynga\Framework\IO\Disk\V1\ManagerInterface as DiskIOManagerInterface;

class ManagerWithFeofFalseOnce extends BaseManager {

  private static ?DiskIOManagerInterface $instance;

  <<__Override>>
  protected function doesFileExist(string $fileName): bool {
    return true;
  }

  <<__Override>>
  protected function isReadable(string $path): bool {
    return true;
  }

  <<__Override>>
  protected function isWriteable(string $path): bool {
    return true;
  }

  <<__Override>>
  protected function fileOpen(string $path, string $mode): mixed {
    return tmpfile();
  }

  <<__Override>>
  protected function bzopen(string $path, string $mode): mixed {
    return tmpfile();
  }

  private static bool $feof = false;
  <<__Override>>
  protected function feof(resource $handle): bool {
    self::$feof = !self::$feof;
    return self::$feof;
  }

}
