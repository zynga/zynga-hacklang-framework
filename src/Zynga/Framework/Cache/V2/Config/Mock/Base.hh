<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\Mock;

use Zynga\Framework\Cache\V2\Config\InMemory\Base as ConfigBase;

use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidExampleObject
;

use Zynga\Framework\Exception\V1\Exception;

abstract class Base extends ConfigBase {

  public function cacheAllowsKeyOverride(): bool {
    return false;
  }

  public function cacheAllowsNonExpiringKeys(): bool {
    return false;
  }

  public function cacheAllowsTTLOverride(): bool {
    return false;
  }

}
