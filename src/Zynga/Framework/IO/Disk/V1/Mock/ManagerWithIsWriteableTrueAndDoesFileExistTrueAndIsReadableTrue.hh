<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Mock;

use Zynga\Framework\IO\Disk\V1\Manager as BaseManager;
use Zynga\Framework\IO\Disk\V1\ManagerInterface as DiskIOManagerInterface;

class ManagerWithIsWriteableTrueAndDoesFileExistTrueAndIsReadableTrue extends BaseManager {

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
  protected function doesFileExist(string $fileName): bool {
    return true;
  }

  <<__Override>>
  protected function isWriteable(string $fileName): bool {
    return true;
  }

  <<__Override>>
  protected function isReadable(string $fileName): bool {
    return true;
  }

}
