<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1\Driver\Caching;

use Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

class StorableToKeyCache {

  private DriverInterface $_driver;
  private Map<string, StorableToKeyCacheEntry> $_cache;

  public function __construct(DriverInterface $driver) {
    $this->_driver = $driver;
    $this->_cache = Map {};
  }

  public function getDriver(): DriverInterface {
    return $this->_driver;
  }

  public function getKeysForStorableObject(
    StorableObjectInterface $obj,
  ): StorableToKeyCacheEntry {

    $hash = spl_object_hash($obj);

    var_dump($this->_cache);

    $cacheEntry = $this->_cache->get($hash);

    if ($cacheEntry instanceof StorableToKeyCacheEntry) {
      return $cacheEntry;
    }

    $cacheEntry = new StorableToKeyCacheEntry($this->getDriver(), $obj);

    $this->_cache->set($hash, $cacheEntry);

    return $cacheEntry;

  }

  public function getCachableKeyForStorableObject(
    StorableObjectInterface $obj,
  ): string {

    $cacheEntry = $this->getKeysForStorableObject($obj);

    return $cacheEntry->getKey();

  }

  public function getCachableLockKeyForStorableObject(
    StorableObjectInterface $obj,
  ): string {

    $cacheEntry = $this->getKeysForStorableObject($obj);

    return $cacheEntry->getLockKey();

  }

}
