<?hh // strict

namespace Zynga\Framework\Cache\V2\Driver;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\Dynamic\V1\DynamicClassCreation;

use Zynga\Framework\Cache\V2\Driver\Base as DriverBase;

use Zynga\Framework\Cache\V2\Driver\Bookkeeping\LockInfo;

use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Cache\V2\Interfaces\DriverInterface;

use Zynga\Framework\Cache\V2\Exceptions\UnableToAcquireLockException;
use Zynga\Framework\Cache\V2\Exceptions\UnlockingWithoutALockException;
use Zynga\Framework\Cache\V2\Exceptions\WriteOperationWithoutALockException;
use Zynga\Framework\Cache\V2\Exceptions\LockTimedOutException;

use Zynga\Framework\Exception\V1\Exception;

use Zynga\Framework\Logging\V1\Factory as LoggingFactory;
use Zynga\Framework\Logging\V1\Interfaces\LoggerInterface;

/**
 * Locks are re-entrant.
 * @extends Memcache
 */
class LockingMemcache extends Memcache {
  const LOGGING_CONFIG = 'GlobalConfig';

  protected Map<string, LockInfo> $lockLookupTable;

  public function __construct(DriverConfigInterface $config) {
    parent::__construct($config);

    $this->lockLookupTable = Map {};
  }

  public function set(StorableObjectInterface $obj): bool {

    try {
      $lockKey = $this->getConfig()->createLockKeyFromStorableObject($obj);
      $this->verifyLockIsValid(__METHOD__, $lockKey);

      return parent::set($obj);
    } catch (Exception $e) {
      throw $e;
    }

  }

  public function delete(StorableObjectInterface $obj): bool {

    try {
      $lockKey = $this->getConfig()->createLockKeyFromStorableObject($obj);
      $this->verifyLockIsValid(__METHOD__, $lockKey);

      return parent::delete($obj);
    } catch (Exception $e) {
      throw $e;
    }
  }

  protected function verifyLockIsValid(string $method, string $lockKey): void {
    if ($this->lockLookupTable->contains($lockKey) === false) {
      throw new WriteOperationWithoutALockException(
        $method,
        'Lock must be established before calling method',
        Map {'key' => $lockKey},
      );
    }

    $lockInfo = $this->lockLookupTable[$lockKey];
    if ($lockInfo->hasTimedOut(time())) {
      throw new LockTimedOutException(
        $method,
        'Lock has timed out. Future writes will fail.',
        Map {
          'key' => $lockKey,
          'timeoutInUnixTime' => $lockInfo->getTimeoutInUnixTime(),
        },
      );
    }

  }

  public function lock(StorableObjectInterface $obj): void {
    try {
      $key = $this->getConfig()->createLockKeyFromStorableObject($obj);

      if ($this->lockLookupTable->containsKey($key)) {
        $this->lockLookupTable[$key]->increment();
        return;
      }

      $this->connect();
      $payload =
        $this->getConfig()->createLockPayloadFromStorableObject($obj);

      $lockFlags = $this->getConfig()->getLockFlags($obj);
      $ttl = $this->getConfig()->getLockTTL($obj);

      set_error_handler(array($this, 'errorHandler'), E_ALL);
      $expireTime = time() + $ttl;
      $success =
        $this->_memcache->add($key, $payload, $lockFlags, $expireTime);
      if ($success === false) {
        throw new UnableToAcquireLockException(
          __METHOD__,
          'Unable to add lock to memcache',
          Map {
            'key' => $key,
            'lockFlags' => $lockFlags,
            'expireTime' => $expireTime,
          },
        );
      }

      $this->lockLookupTable[$key] = new LockInfo(1, $expireTime);
    } catch (Exception $e) {
      throw $e;
    } finally {
      restore_error_handler();
    }
  }

  public function unlock(StorableObjectInterface $obj): void {
    try {
      $key = $this->getConfig()->createLockKeyFromStorableObject($obj);

      if ($this->lockLookupTable->contains($key) === true) {
        $this->lockLookupTable[$key]->decrement();
        if ($this->lockLookupTable[$key]->hasNoLocks() === false) {
          return;
        }
      } else {
        throw new UnlockingWithoutALockException(
          __METHOD__,
          'Key is not locked. Cannot unlock without a lock around the key.',
          Map {'key' => $key},
        );
      }

      $this->connect();

      set_error_handler(array($this, 'errorHandler'), E_ALL);

      if ($this->lockLookupTable[$key]->hasTimedOut(time()) === false) {
        // This can fail if Memcache is not accessible or if the key is already deleted.
        // In either case, we only care that the key is gone after this.
        $this->_memcache->delete($key);

        $result = $this->_memcache->get($key);
        if ($result !== null) {
          $logger = LoggingFactory::factory(
            LoggerInterface::class,
            static::LOGGING_CONFIG,
          );
          $logger->warning(
            'Key was not deleted after unlock.',
            Map {'key' => $key, 'method' => __METHOD__},
          );
        }
      }

      $this->lockLookupTable->remove($key);
    } catch (Exception $e) {
      throw $e;
    } finally {
      restore_error_handler();
    }
  }

  /**
   * Don't make a scene if the memcache extension tells us that it is retrying...
   */
  protected function errorHandler(): bool {
    $error = func_get_args();
    if ($error[0] === E_WARNING &&
        false !== strpos(
          $error[1],
          '/failed with: Connection Error. Retry attempt/',
        )) {
      return false;
    }
    throw new Exception($error[1]);
  }
}
