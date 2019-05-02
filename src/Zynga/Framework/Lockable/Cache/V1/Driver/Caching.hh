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

      $lockKey =
        $this->getKeyCache()->getCachableLockKeyForStorableObject($obj);

      var_dump($lockKey);
      $alreadyLocked = $this->_locks->get($lockKey);

      // if our own lock has expired through neglect then its time to re-add it.
      if ($alreadyLocked instanceof LockPayloadInterface) {
        if ($alreadyLocked->isLockStillValid(
              $this->getConfig()->getLockTTL(),
            )) {
          $this->_locks->remove($lockKey);
        }
      }

      $cache = $this->getConfig()->getCache();

      $lockPayload = $this->getConfig()->getPayloadObject();

      $addResult = $cache->add($lockPayload, $lockKey);

      if ($addResult === true) {
        $this->_locks->set($lockKey, $lockPayload);
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

      $lockKey =
        $this->getKeyCache()->getCachableLockKeyForStorableObject($obj);

      $alreadyLocked = $this->_locks->get($lockKey);

      // if we are not the owner of a lock, we cannot do a unlock op, so shortcut the unlock if needed.
      if (!$alreadyLocked instanceof LockPayloadInterface) {
        return true;
      }

      $cache = $this->getConfig()->getCache();

      $deleteResult = $cache->delete($obj, $lockKey);

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
   * @param bool $getLocked Establish a lock for he object at the sime time a get is done.
   * @return bool object return
   */
  public function get(
    StorableObjectInterface $obj,
    bool $getLocked = false,
  ): ?StorableObjectInterface {

    try {

      $lockKey =
        $this->getKeyCache()->getCachableLockKeyForStorableObject($obj);

      if ($getLocked === true && $this->lock($obj) !== true) {
        // could not establish a lock, read consistency not guarenteed.
        throw new UnableToEstablishLockException(
          'Unable to establish lock lockKey='.$lockKey,
        );
      }

      $cache = $this->getConfig()->getCache();

      $cacheKey = $this->getKeyCache()->getCachableKeyForStorableObject($obj);

      $obj = $cache->get($obj, $cacheKey);

      error_log('isCached get obj='.var_export($obj, true));
      return $obj;

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

      $lockKey =
        $this->getKeyCache()->getCachableLockKeyForStorableObject($obj);

      if ($this->lock($obj) !== true) {
        throw new UnableToEstablishLockException(
          'Unable to establish lock lockKey='.$lockKey,
        );
      }

      $cache = $this->getConfig()->getCache();

      $cacheKey = $this->getKeyCache()->getCachableKeyForStorableObject($obj);

      $setSuccess = $cache->set($obj, $cacheKey);

      // We failed to set so the lock needs to persist.
      if ($setSuccess == false) {
        error_log('isCached setSucces=false');
        return false;
      }

      // Release the lock as we are done here.
      $unlockReturn = $this->unlock($obj);

      error_log(
        'isCached setSucces=true, unlockReturn='.json_encode($unlockReturn),
      );

      return $unlockReturn;

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

      $lockKey =
        $this->getKeyCache()->getCachableLockKeyForStorableObject($obj);

      if ($this->lock($obj) !== true) {
        throw new UnableToEstablishLockException(
          'Unable to establish lock lockKey='.$lockKey,
        );
      }

      $cache = $this->getConfig()->getCache();

      $cacheKey = $this->getKeyCache()->getCachableKeyForStorableObject($obj);

      $deleteState = $cache->delete($obj, $cacheKey);

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
