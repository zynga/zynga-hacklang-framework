<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1\Interfaces;

use
  Zynga\Framework\Cache\V2\Interfaces\DriverInterface as CacheDriverInterface
;
use Zynga\Framework\Lockable\Cache\V1\Interfaces\LockPayloadInterface;
use Zynga\Framework\Factory\V2\Interfaces\ConfigInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

interface DriverConfigInterface extends ConfigInterface {

  /**
   *
   * Return the appropriate cache object here.
   *
   * @return CacheDriverInterface
   *
   */
  public function getCache(): CacheDriverInterface;

  /**
   *
   * This creates the key that is passed down into your caching layer.
   *
   * @param string storableObjectKeyString
   * @return string key
   *
   */
  public function createLockKeyFromStorableKey(string $obj): string;

  /**
   *
   * Most implementations use the default LockPayload, but you can overload it
   * if needed.
   *
   * @return LockPayLoadInterface
   *
   */
  public function getPayloadObject(): LockPayloadInterface;

  /**
   *
   * Defaults to the cache's TTL value, if you want it to be shorter
   * overload it here, however we do check that you are not setting a
   * value that is higher than the cache's supported TTL.
   *
   * @return int number of seconds
   *
   */
  public function getLockTTL(): int;

}
