<?hh // strict

namespace Zynga\Framework\IO\Disk\V1\Mock;

use Zynga\Framework\IO\Disk\V1\Mock\ManagerWithFeofFalseOnce as BaseManager;
use Zynga\Framework\IO\Disk\V1\ManagerInterface as DiskIOManagerInterface;

class ManagerWithFeofFalseOnceFcloseFails extends BaseManager {

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
  protected function fclose(resource $handle): bool {
    return false;
  }

}
