<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\Mock\NoCacheKeyOverride;

use Zynga\Framework\Cache\V2\Config\Mock\Base as MockBase;

use Zynga\Framework\Exception\V1\Exception;

class Dev extends MockBase {

  public function cacheAllowsKeyOverride(): bool {
    return false;
  }

}
