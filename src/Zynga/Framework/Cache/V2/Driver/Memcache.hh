<?hh // strict

namespace Zynga\Framework\Cache\V2\Driver;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\Dynamic\V1\DynamicClassCreation;

use Zynga\Framework\Cache\V2\Driver\Base as DriverBase;
use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Cache\V2\Interfaces\DriverInterface;
use Zynga\Framework\Cache\V2\Exceptions\NoServerPairsProvidedException;
use Zynga\Framework\Cache\V2\Exceptions\NoConnectionException;
use Zynga\Framework\Cache\V2\Exceptions\StorableObjectRequiredException;

use Zynga\Framework\Exception\V1\Exception;

use \Memcache as NativeMemcacheDriver;

class Memcache extends DriverBase {
  private NativeMemcacheDriver $_memcache;
  private DriverConfigInterface $_config;

  public function __construct(DriverConfigInterface $config) {
    $this->_config = $config;
    $this->_memcache = new NativeMemcacheDriver();
  }

  public function getConfig(): DriverConfigInterface {
    return $this->_config;
  }

  public function directIncrement(string $key, int $incrementValue = 1): int {

    try {

      $this->connect();

      if ($incrementValue < 0 || $incrementValue == 0) {
        throw new Exception('Increment value must be greater than 0');
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
      //   always returns true as it lazy connects at use time.
      foreach ($serverPairs as $host => $port) {
        error_log('addServer host='.$host.' port='.$port);
        $memcache->addserver($host, $port);
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

      error_log(
        'cache::get isCached get key='.
        $key.
        ' mc='.
        get_class($this->_memcache),
      );

      $data = $this->_memcache->get($key);

      // no data to work with.
      if ($data === false) {
        error_log('cache::get isCached data=null rawData='.$data);
        return null;
      }

      error_log('isCached data='.$data);
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

      var_dump($key);

      $flags = 0;
      $success = $this->_memcache->set($key, $jsonValue, $flags, $ttl);

      error_log(
        'isCached set key='.
        $key.
        ' jsonValue='.
        $jsonValue.
        ' success='.
        var_export($jsonValue, true).
        ' flags='.
        $flags.
        ' ttl='.
        $ttl.
        ' currentTime='.
        time(),
      );

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

      error_log('JEO MCDriver key='.$key.' delete='.var_export($success));

      if ($success == 1) {
        error_log('JEO MCDriver delete-AOK');
        return true;
      }

      error_log('JEO MCDriver delete-FAIL');
      return false;

    } catch (Exception $e) {
      throw $e;
    }

  }

}
