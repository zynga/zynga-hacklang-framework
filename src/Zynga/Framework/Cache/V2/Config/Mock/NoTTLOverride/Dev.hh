<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\Mock\NoTTLOverride;

use Zynga\Framework\Cache\V2\Config\LocalMemcache\Base as LocalMemcacheBase;
use Zynga\Framework\Cache\V2\Exceptions\InvalidObjectForKeyCreationException;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidExampleObject
;

use Zynga\Framework\Exception\V1\Exception;

class Dev extends LocalMemcacheBase {

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
