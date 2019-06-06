<?hh // strict

namespace Zynga\Framework\IO\Disk\V1;

use \Exception;
use Zynga\Framework\IO\Disk\V1\Manager\UNIX;
use Zynga\Framework\IO\Disk\V1\Interfaces\ManagerInterface;

/**
 * Lightweight class for managing Disk IO
 */
class Manager {

  private static ?ManagerInterface $instance;

  public static function instance(): ManagerInterface {

    if (self::$instance instanceof ManagerInterface) {
      return self::$instance;
    }

    self::$instance = new UNIX();

    return self::$instance;

  }

  public static function clear(): bool {
    self::$instance = null;
    return true;
  }

  // --
  // JEO: We didn't need this after all.
  // --
  // public static function setManagerInstance(ManagerInterface $obj): bool {
  //   self::$instance = $obj;
  //   return true;
  // }

}
