<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\Mock\InMemory;

use Zynga\Framework\Cache\V2\Config\InMemory\Base as InMemoryBase;

use \Exception;

abstract class PgDataBase extends InMemoryBase {

  public function cacheAllowsKeyOverride(): bool {
    return true;
  }
}
