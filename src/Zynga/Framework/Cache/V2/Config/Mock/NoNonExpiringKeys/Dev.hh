<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\Mock\NoNonExpiringKeys;

use Zynga\Framework\Cache\V2\Config\Mock\Base as MockBase;

use Zynga\Framework\Exception\V1\Exception;

class Dev extends MockBase {

  public function cacheAllowsTTLOverride(): bool {
    return true;
  }

  public function cacheAllowsNonExpiringKeys(): bool {
    return false;
  }

}
