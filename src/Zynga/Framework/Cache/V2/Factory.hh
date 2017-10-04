<?hh // strict

namespace Zynga\Framework\Cache\V2;

use Zynga\Framework\Factory\V2\Base as FactoryBase;

/**
 * Caching layer factory, handles delivering Cache\DriverInterface to your hands.
 */
class Factory extends FactoryBase {

  public static function getClassRoot(): string {
    return '\Zynga\Framework\Cache\V2';
  }

}
