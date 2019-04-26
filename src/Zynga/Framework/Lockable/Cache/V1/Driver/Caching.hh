<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1\Driver;

use Zynga\Framework\Lockable\Cache\V1\Driver\Caching\StorableToKeyCache;
use Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface;
use Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverConfigInterface;
use Zynga\Framework\Lockable\Cache\V1\Interfaces\LockPayloadInterface;
use
  Zynga\Framework\Lockable\Cache\V1\Exceptions\UnableToEstablishLockException
;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\Factory\V2\Driver\Base as FactoryDriverBase;

use \Exception;

class Caching extends FactoryDriverBase implements DriverInterface {
  private ?StorableToKeyCache $_keyCache;
  private DriverConfigInterface $_config;
  private Map<string, LockPayloadInterface> $_locks;

  public function __construct(DriverConfigInterface $config) {
    $this->_keyCache = null;
    $this->_config = $config;
    $this->_locks = Map {};
  }

  /**
   *
   * Fetches the configuration for this driver instance.
   *
   * @return DriverConfigInterface
   */
  public function getConfig(): DriverConfigInterface {
    return $this->_config;
  }

  public function getKeyCache(): StorableToKeyCache {

    if ($this->_keyCache instanceof StorableToKeyCache) {
      return $this->_keyCache;
    }

    $this->_keyCache = new StorableToKeyCache($this);
    return $this->_keyCache;

  }

  /**
   *
   * Locking a existing object if possible, exception if not capable.
   *
   * @param StorableObjectInterface $obj
   * @param bool lock success state
   *
   */
  public function lock(StorableObjectInterface $obj): bool {

    try {

      $key = $this->getKeyCache()->getHashForStorableObject($obj);

      $alreadyLocked = $this->_locks->get($key);

      // if our own lock has expired through neglect then its time to re-add it.
      if ($alreadyLocked instanceof LockPayloadInterface) {
        if ($alreadyLocked->isLockStillValid(
              $this->getConfig()->getLockTTL(),
            )) {
          $this->_locks->remove($key);
        }
      }

      $cache = $this->getConfig()->getCache();

      $lockPayload = $this->getConfig()->getPayloadObject();

      $addResult = $cache->add($lockPayload, $key);

      if ($addResult === true) {
        $this->_locks->set($key, $lockPayload);
        return true;
      }

      return false;

    } catch (Exception $e) {
      throw $e;
    }

  }

  /**
   *
   * Given a existing object that is locked, unlock it.
   * If we didn't have a lock on the object, toss an exception about the issue.
   *
   * @param StorableObjectInterface $obj
   * @return bool unlock success state
   *
   */
  public function unlock(StorableObjectInterface $obj): bool {

    try {

      $key = $this->getKeyCache()->getHashForStorableObject($obj);

      $alreadyLocked = $this->_locks->get($key);

      // if we are not the owner of a lock, we cannot do a unlock op, so shortcut the unlock if needed.
      if (!$alreadyLocked instanceof LockPayloadInterface) {
        return true;
      }

      $cache = $this->getConfig()->getCache();

      $deleteResult = $cache->delete($obj, $key);

      if ($deleteResult === true) {
        return true;
      }

      return false;

    } catch (Exception $e) {
      throw $e;
    }

  }

  /**
   *
   * We assume you want the object locked as part of this api, so the lock is
   * established on fetch.
   *
   * @param StorableObjectInterface $obj
   * @return bool object return
   */
  public function get(StorableObjectInterface $obj): ?StorableObjectInterface {

    try {

      if ($this->lock($obj) !== true) {
        // could not establish a lock, read consistency not guarenteed.
        throw new UnableToEstablishLockException(
          'Unable to establish lock lockKey='.
          $this->getKeyCache()->getHashForStorableObject($obj),
        );
      }

      $cache = $this->getConfig()->getCache();

      return $cache->get($obj);

    } catch (Exception $e) {
      throw $e;
    }

  }

  /**
   *
   * If the object isn't locked previously via a manual lock() moment,
   * you will get an exception on set.
   *
   * @param bool releaseLockOnSet releases the lock upon a successful set moment. defaults to always.
   * @return bool set success
   *
   */
  public function set(
    StorableObjectInterface $obj,
    bool $releaseLockOnSet = true,
  ): bool {

    try {

      if ($this->lock($obj) !== true) {
        throw new UnableToEstablishLockException(
          'Unable to establish lock lockKey='.
          $this->getKeyCache()->getHashForStorableObject($obj),
        );
      }

      $cache = $this->getConfig()->getCache();

      $setSuccess = $cache->set($obj);

      // We failed to set so the lock needs to persist.
      if ($setSuccess == false) {
        return false;
      }

      // Release the lock as we are done here.
      return $this->unlock($obj);

    } catch (Exception $e) {
      throw $e;
    }

  }

  /**
   *
   * If the object isn't locked previously via a manual lock() moment,
   * you will get an exeception on set.
   *
   * @param StorableObjectInterface $obj object you would like deleted.
   * @param bool $releaseLockOnDelete releases the lock upon a succcessful lock moment, defaults to always.
   * @return bool delete success.
   *
   */
  public function delete(
    StorableObjectInterface $obj,
    bool $releaseLockOnDelete = true,
  ): bool {
    try {

      if ($this->lock($obj) !== true) {
        throw new UnableToEstablishLockException(
          'Unable to establish lock lockKey='.
          $this->getKeyCache()->getHashForStorableObject($obj),
        );
      }

      $cache = $this->getConfig()->getCache();

      $deleteState = $cache->delete($obj);

      // Failed to delete the object from cache, we shouldn't release the lock.
      if ($deleteState != true) {
        return false;
      }

      // Purge the lock as the object is now removed also.
      return $this->unlock($obj);

    } catch (Exception $e) {
      throw $e;
    }
  }

}
