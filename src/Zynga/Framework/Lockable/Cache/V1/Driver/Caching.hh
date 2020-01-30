<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1\Driver;

use Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface;
use Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverConfigInterface;
use Zynga\Framework\Lockable\Cache\V1\Interfaces\LockPayloadInterface;
use
  Zynga\Framework\Lockable\Cache\V1\Exceptions\UnableToEstablishLockException
;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\Factory\V2\Driver\Base as FactoryDriverBase;
use Zynga\Framework\Lockable\Cache\V1\LockPayload;

use \Exception;

class Caching extends FactoryDriverBase implements DriverInterface {
  private DriverConfigInterface $_config;
  private Map<string, LockPayloadInterface> $_locks;
  
  // Lock operation would try a max of 100 times and sleep for 10000 in between
  const int LOCK_ATTEMPTS_MAX = 100;
  const int LOCK_TIMEOUT_AMOUNT_MICRO_SECONDS = 10000;
  // Max lock timeout is 1 seconds
  const int MAX_TIMEOUT_AMOUNT_SECONDS = 1;
  
  public function __construct(DriverConfigInterface $config) {
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

  public function getActiveLocks(): Map<string, LockPayloadInterface> {
    return $this->_locks;
  }

  public function getCacheKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {
    try {
      $cache = $this->getConfig()->getCache();
      return $cache->getConfig()->createKeyFromStorableObject($obj);
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function getLockCacheKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {
    try {
      $cache = $this->getConfig()->getLockCache();
      return $cache->getConfig()->createKeyFromStorableObject($obj);
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function isLockedByMyThread(StorableObjectInterface $obj): bool {

    try {
      $lockKey = $this->getLockCacheKeyFromStorableObject($obj);
      $alreadyLocked = $this->_locks->get($lockKey);

      // Check my thead already has a lock
      if ($alreadyLocked instanceof LockPayloadInterface) {
        // Check if the lock is still valid, if so we are done.
        if ($alreadyLocked->isLockStillValid(
              $this->getConfig()->getLockTTL(),
            )) {
          return true;
        }
      }
      return false;
    } catch (Exception $e) {
      throw $e;
    }

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

      $lockKey = $this->getLockCacheKeyFromStorableObject($obj);
      $alreadyLocked = $this->_locks->get($lockKey);

      // if our own lock has expired through neglect then its time to re-add it.
      if ($alreadyLocked instanceof LockPayloadInterface) {
        // Check if the lock is still valid, if so we are done.
        if ($alreadyLocked->isLockStillValid(
              $this->getConfig()->getLockTTL(),
            )) {
          return true;
        }

        // purge the key from the local map and allow us to establish a new lock.
        $this->_locks->remove($lockKey);

      }
      
      $lockCache = $this->getConfig()->getLockCache();
      $lockPayload = $this->getConfig()->getPayloadObject();
      
      $startTime = microtime(true);
      $lockRetryCount = 0;
      
      for ( $lockRetryCount = 0; $lockRetryCount < self::LOCK_ATTEMPTS_MAX; $lockRetryCount++ ) {
        $lockPayload->setLockEstablishment(time());
        $lockPayload->setLockRetryCount($lockRetryCount);
        $addResult = $lockCache->add($lockPayload, $lockKey);
        if ($addResult === true) {
          $this->_locks->set($lockKey, $lockPayload);
          return true;
        }
        
        $lockRetryCount++;
        
        // We crossed max timeout, break early.
        if( (microtime(true) - $startTime) > self::MAX_TIMEOUT_AMOUNT_SECONDS) {
          return false;
        }
        
        // Don't care about the first lock try. Only log an error for subsequent ones.
        error_log('JEO pgData::lock storable=' . get_class($obj) . ' key=' . $lockKey . ' lockRetryCount=' . $lockRetryCount . ' obj=' . $obj->export()->asJSON());
        // if failed to get a lock, sleep and try again
        usleep(self::LOCK_TIMEOUT_AMOUNT_MICRO_SECONDS);
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

      $lockKey = $this->getLockCacheKeyFromStorableObject($obj);

      $alreadyLocked = $this->_locks->get($lockKey);

      // if we are not the owner of a lock, we cannot do a unlock op, so shortcut the unlock if needed.
      if (!$alreadyLocked instanceof LockPayloadInterface) {
        return false;
      }

      $lockCache = $this->getConfig()->getLockCache();

      $deleteResult = $lockCache->delete($obj, $lockKey);

      $this->_locks->remove($lockKey);

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

      $lockKey = $this->getLockCacheKeyFromStorableObject($obj);

      if ($getLocked === true && $this->lock($obj) !== true) {
        // could not establish a lock, read consistency not guarenteed.
        throw new UnableToEstablishLockException(
          'Unable to establish lock lockKey='.$lockKey,
        );
      }

      $cache = $this->getConfig()->getCache();

      $cacheKey = $this->getCacheKeyFromStorableObject($obj);

      $obj = $cache->get($obj, $cacheKey);

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

      $lockKey = $this->getLockCacheKeyFromStorableObject($obj);

      if ($this->lock($obj) !== true) {
        throw new UnableToEstablishLockException(
          'Unable to establish lock lockKey='.$lockKey,
        );
      }

      $cache = $this->getConfig()->getCache();

      $cacheKey = $this->getCacheKeyFromStorableObject($obj);

      $setSuccess = $cache->set($obj, $cacheKey);

      // We failed to set so the lock needs to persist.
      //if ($setSuccess == false) {
      //  return false;
      //}

      // Release the lock as we are done here.
      $unlockSuccess = true;
      if ($releaseLockOnSet === true) {
        $unlockSuccess = $this->unlock($obj);
      }

      return $setSuccess || $unlockSuccess;

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

      $lockKey = $this->getLockCacheKeyFromStorableObject($obj);

      if ($this->lock($obj) !== true) {
        throw new UnableToEstablishLockException(
          'Unable to establish lock lockKey='.$lockKey,
        );
      }

      $cache = $this->getConfig()->getCache();

      $cacheKey = $this->getCacheKeyFromStorableObject($obj);

      $deleteState = $cache->delete($obj, $cacheKey);

      // Failed to delete the object from cache, we shouldn't release the lock.
      if ($deleteState != true) {
        return false;
      }

      if ($releaseLockOnDelete === true) {
        // Purge the lock as the object is now removed also.
        return $this->unlock($obj);
      }

      return true;
    } catch (Exception $e) {
      throw $e;
    }

  }

}
