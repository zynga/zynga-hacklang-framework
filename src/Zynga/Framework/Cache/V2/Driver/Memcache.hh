<?hh // strict

namespace Zynga\Framework\Cache\V2\Driver;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\DynamicClassCreation\V1\DynamicClassCreation;

use Zynga\Framework\Cache\V2\Driver\Base as DriverBase;
use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Cache\V2\Interfaces\DriverInterface;
use Zynga\Framework\Cache\V2\Exceptions\NoServerPairsProvidedException;
use Zynga\Framework\Cache\V2\Exceptions\NoConnectionException;
use Zynga\Framework\Cache\V2\Exceptions\StorableObjectRequiredException;

use Zynga\Exception;

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
        $memcache->addserver($host, $port);
      }

      $this->_memcache = $memcache;

      return true;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function getByMap(
    Map<string, mixed> $data,
  ): ?StorableObjectInterface {

    try {

      $className = $this->getConfig()->getStorableObjectName();

      $obj = DynamicClassCreation::createClassByName($className, Vector {});

      if (!$obj instanceof StorableObjectInterface) {
        throw new StorableObjectRequiredException('className='.$className);
      }

      $obj->import()->fromMap($data);

      return $this->get($obj);

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function get(StorableObjectInterface $obj): ?StorableObjectInterface {

    try {

      $key = $this->getConfig()->createKeyFromStorableObject($obj);

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

  public function set(StorableObjectInterface $obj): bool {

    try {

      $key = $this->getConfig()->createKeyFromStorableObject($obj);

      $this->connect();

      $flags = 0;

      $ttl = time() + $this->getConfig()->getTTL();

      $success =
        $this->_memcache->set($key, $obj->export()->asJSON(), $flags, $ttl);

      return $success;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function deleteByMap(Map<string, mixed> $data): bool {

    try {

      $className = $this->getConfig()->getStorableObjectName();

      $obj = DynamicClassCreation::createClassByName($className, Vector {});

      if (!$obj instanceof StorableObjectInterface) {
        throw new StorableObjectRequiredException('className='.$className);
      }

      $obj->import()->fromMap($data);

      return $this->delete($obj);

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function delete(StorableObjectInterface $obj): bool {

    try {

      $key = $this->getConfig()->createKeyFromStorableObject($obj);

      $this->connect();

      $success = $this->_memcache->delete($key);

      return $success;

    } catch (Exception $e) {
      throw $e;
    }

  }

}
