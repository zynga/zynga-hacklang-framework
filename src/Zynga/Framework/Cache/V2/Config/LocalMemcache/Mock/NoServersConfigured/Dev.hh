<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\LocalMemcache\Mock\NoServersConfigured;

use Zynga\Framework\Cache\V2\Config\LocalMemcache\Base as LMCBase;

// --
// This mock overrides the config with no servers to force a failed connection test for both the local
// memcache setup and the memcache driver itself.
// --
class Dev extends LMCBase {

  public function getServerPairings(): Map<string, int> {
    $hosts = Map {};
    return $hosts;
  }

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
