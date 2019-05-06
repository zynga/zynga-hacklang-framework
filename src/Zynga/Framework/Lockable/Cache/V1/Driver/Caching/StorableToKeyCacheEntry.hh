<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1\Driver\Caching;

use Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

class StorableToKeyCacheEntry {
  private string $_key;
  private string $_lockKey;

  public function __construct(
    DriverInterface $driver,
    StorableObjectInterface $obj,
  ) {

    $this->_key =
      $driver->getConfig()
        ->getCache()
        ->getConfig()
        ->createKeyFromStorableObject($obj);

    $this->_lockKey =
      $driver->getConfig()->createLockKeyFromStorableKey($this->_key);

  }

  public function getKey(): string {
    return $this->_key;
  }

  public function getLockKey(): string {
    return $this->_lockKey;
  }

}
