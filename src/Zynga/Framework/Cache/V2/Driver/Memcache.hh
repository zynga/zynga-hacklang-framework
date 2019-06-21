<?hh // strict

namespace Zynga\Framework\Cache\V2\Driver;

use Zynga\Framework\Cache\V2\Driver\Base as DriverBase;
use Zynga\Framework\Cache\V2\Exceptions\InvalidIncrementStepException;
use Zynga\Framework\Cache\V2\Exceptions\NoServerPairsProvidedException;
use Zynga\Framework\Cache\V2\Exceptions\NoConnectionException;
use Zynga\Framework\Cache\V2\Exceptions\StorableObjectRequiredException;
use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Cache\V2\Interfaces\DriverInterface;
use Zynga\Framework\Dynamic\V1\DynamicClassCreation;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use \Memcache as NativeMemcacheDriver;

class Memcache extends DriverBase {
  private NativeMemcacheDriver $_memcache;
  private DriverConfigInterface $_config;
  // Map used to keep track of hosts that have been registered to avoid duplicates
  private Map<string, int> $_registeredHosts;

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

      $success = $this->_memcache->set($key, $value, $flags, $ttl);

      return $success;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function directGet(string $key, int $count = 1): array<mixed> {
    try {
      $this->connect();

      $keyArray = array($key, $count);
      $items = $this->_memcache->get($keyArray);

      return $items;
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function directDelete(string $key): bool {

    try {

      $this->connect();

      $value = $this->_memcache->delete($key);

      if ($value == true) {
        return true;
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

      $data = $this->_memcache->get($key);

      // no data to work with.
      if ($data === false) {
        return null;
      }

      $obj->import()->fromJSON($data);

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

  public function close(): bool {
    $closed = $this->_memcache->close();
    return $closed;
  }

}
