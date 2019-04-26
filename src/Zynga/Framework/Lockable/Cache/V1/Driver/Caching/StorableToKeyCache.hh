<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1\Driver\Caching;

use Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

class StorableToKeyCache {

  private DriverInterface $_driver;
  private Map<string, string> $_cache;

  public function __construct(DriverInterface $driver) {
    $this->_driver = $driver;
    $this->_cache = Map {};
  }

  public function getDriver(): DriverInterface {
    return $this->_driver;
  }

  public function getHashForStorableObject(
    StorableObjectInterface $obj,
  ): string {

    $hash = spl_object_hash($obj);

    $key = $this->_cache->get($hash);

    if (is_string($key)) {
      return $key;
    }

    $key =
      $this->getDriver()
        ->getConfig()
        ->getCache()
        ->getConfig()
        ->createKeyFromStorableObject($obj);

    $this->_cache->set($hash, $key);

    return $key;

  }

}
