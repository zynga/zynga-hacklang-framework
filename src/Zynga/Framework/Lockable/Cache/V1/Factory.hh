<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1;

use Zynga\Framework\Factory\V2\Base as FactoryBase;

/**
 * Caching layer factory, handles delivering Cache\DriverInterface to your hands.
 */
class Factory extends FactoryBase {

  public static function getClassRoot(): string {
    return '\Zynga\Framework\Lockable\Cache\V1';
  }

}
