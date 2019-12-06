<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\Mock;

use Zynga\Framework\Cache\V2\Config\Mock\Base as MockBase;

class Dev extends MockBase {

  // --
  // For dev|staging|production we allow all the flags to be on to mimic a permissive cache.
  // --
  public function cacheAllowsKeyOverride(): bool {
    return true;
  }

  public function cacheAllowsNonExpiringKeys(): bool {
    return true;
  }

  public function cacheAllowsTTLOverride(): bool {
    return true;
  }

}
