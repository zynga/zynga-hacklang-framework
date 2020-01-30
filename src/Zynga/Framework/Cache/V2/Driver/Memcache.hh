<?hh // strict

namespace Zynga\Framework\Cache\V2\Driver;

use Zynga\Framework\Cache\V2\Driver\Base as DriverBase;
use Zynga\Framework\Cache\V2\Exceptions\InvalidIncrementStepException;
use Zynga\Framework\Cache\V2\Exceptions\NoServerPairsProvidedException;
use Zynga\Framework\Cache\V2\Exceptions\NoConnectionException;
use Zynga\Framework\Cache\V2\Exceptions\StorableObjectRequiredException;
use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Cache\V2\Interfaces\MemcacheDriverInterface;
use Zynga\Framework\Cache\V2\Interfaces\DriverInterface;
use Zynga\Framework\Dynamic\V1\DynamicClassCreation;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use \Memcache as NativeMemcacheDriver;

class Memcache extends DriverBase implements MemcacheDriverInterface {
  private NativeMemcacheDriver $_memcache;
  private DriverConfigInterface $_config;
  // Map used to keep track of hosts that have been registered to avoid duplicates
  private Map<string, int> $_registeredHosts;
  
  // Operations like set and delete will retry their operation
  const int OPERATION_ATTEMPTS_MAX = 100; // 100 * 10000 = 1s max wait time.
  const int OPERATION_TIMEOUT_AMOUNT_MICRO_SECONDS = 10000;
  
  // Max lock timeout is 1 seconds
  const int MAX_TIMEOUT_AMOUNT_SECONDS = 1;

  public function __construct(DriverConfigInterface $config) {
    $this->_config = $config;
    $this->_memcache = new NativeMemcacheDriver();
    $this->_registeredHosts = Map {};
  }

  public function getConfig(): DriverConfigInterface {
    return $this->_config;
  }

  public function directIncrement(string $key, int $incrementValue = 1): int {

    try {

      $this->connect();

      if ($incrementValue < 0 || $incrementValue == 0) {
        throw new InvalidIncrementStepException(
          'Increment value must be greater than 0',
        );
      }

      $value = $this->_memcache->increment($key, $incrementValue);

      if ($value === false) {
        return 0;
      }

      return $value;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function directAdd(
    string $key,
    mixed $value,
    int $flags = 0,
    int $ttl = 0,
  ): bool {

    try {

      $this->connect();

      $value = $this->_memcache->add($key, $value, $flags, $ttl);

      if ($value == true) {
        return true;
      }

      return false;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function directSet(
    string $key,
    mixed $value,
    int $flags = 0,
    int $ttl = 0,
  ): bool {

    try {

      $this->connect();

      $startTime = microtime(true);
      for ($retryCount = 0; $retryCount < self::OPERATION_ATTEMPTS_MAX; $retryCount++) {
        $success = $this->_memcache->set($key, $value, $flags, $ttl);
        if ($success == true) {
          return true;
        }
        
        // We crossed max timeout, break early.
        if( (microtime(true) - $startTime) > self::MAX_TIMEOUT_AMOUNT_SECONDS) {
          return false;
        }
        
        usleep(self::OPERATION_TIMEOUT_AMOUNT_MICRO_SECONDS);
      }
      
      return false;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function directGet(string $key): mixed {
    try {
      $this->connect();

      $item = $this->_memcache->get($key);

      return $item;
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function directDelete(string $key): bool {

    try {

      $this->connect();
      
      $startTime = microtime(true);
      for ($retryCount = 0; $retryCount < self::OPERATION_ATTEMPTS_MAX; $retryCount++) {
        $success = $this->_memcache->delete($key);
        if ($success == true) {
          return true;
        }
        
        // We crossed max timeout, break early.
        if( (microtime(true) - $startTime) > self::MAX_TIMEOUT_AMOUNT_SECONDS) {
          return false;
        }
        
        usleep(self::OPERATION_TIMEOUT_AMOUNT_MICRO_SECONDS);
      }
      

      return false;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function connect(): bool {

    // --
    // sadly they don't provide a way to see if the connection has been
    // configured. So you jiggle getversion to see if there is anything
    // configured. SAD PANDA!
    // --
    $versionInfo = $this->_memcache->getversion();

    if ($this->_memcache->getversion() !== false) {
      return true;
    }

    $memcache = $this->_memcache;

    try {

      $serverPairs = $this->getConfig()->getServerPairings();

      if ($serverPairs->count() == 0) {
        throw new NoServerPairsProvidedException(
          'config='.get_class($this->getConfig()),
        );
      }

      // add the host / port combinations to the memcache object, addserver
      // always returns true as it lazy connects at use time.
      foreach ($serverPairs as $host => $port) {

        // addserver does not check for duplicates
        if ($this->_registeredHosts->containsKey($host) === false) {
          $memcache->addserver($host, $port);
          $this->_registeredHosts[$host] = $port;
        }
      }

      $this->_memcache = $memcache;

      return true;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function add(
    StorableObjectInterface $obj,
    string $keyOverride = '',
    int $ttlOverride = -1,
  ): bool {

    try {

      $key = $this->getKeySupportingOverride($obj, $keyOverride);
      $ttl = $this->getTTLSupportingOverride($ttlOverride);

      $jsonValue = $obj->export()->asJSON();

      $flags = 0;
      $return = $this->directAdd($key, $jsonValue, $flags, $ttl);

      if ($return == true) {
        return true;
      }

      return false;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function get(
    StorableObjectInterface $obj,
    string $keyOverride = '',
  ): ?StorableObjectInterface {

    try {

      $key = $this->getKeySupportingOverride($obj, $keyOverride);

      $this->connect();

      $data = $this->directGet($key);

      // no data to work with.
      if ($data === false) {
        return null;
      }

      $obj->import()->fromJSON(strval($data));

      return $obj;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function set(
    StorableObjectInterface $obj,
    string $keyOverride = '',
    int $ttlOverride = -1,
  ): bool {

    try {

      $key = $this->getKeySupportingOverride($obj, $keyOverride);
      $ttl = $this->getTTLSupportingOverride($ttlOverride);

      $this->connect();

      $jsonValue = $obj->export()->asJSON();

      $flags = 0;
      $success = $this->directSet($key, $jsonValue, $flags, $ttl);

      return $success;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function delete(
    StorableObjectInterface $obj,
    string $keyOverride = '',
  ): bool {

    try {

      $key = $this->getKeySupportingOverride($obj, $keyOverride);

      $this->connect();

      $success = $this->_memcache->delete($key);

      if ($success == 1) {
        return true;
      }

      return false;

    } catch (Exception $e) {
      throw $e;
    }

  }

}
