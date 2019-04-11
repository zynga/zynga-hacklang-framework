<?hh // strict

namespace Zynga\Framework\Cache\V2\Driver;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\Dynamic\V1\DynamicClassCreation;
use Zynga\Framework\Cache\V2\Driver\Base as DriverBase;
use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Cache\V2\Interfaces\DriverInterface;
use Zynga\Framework\Cache\V2\Exceptions\StorableObjectRequiredException;
use Zynga\Framework\Cache\V2\Driver\InMemory as InMemoryDriver;
use Zynga\Framework\Exception\V1\Exception;

/**
 * This is an in-memory cache for transient data. If you need to have data persist between requests, consider the Memcache driver
 */
class InMemory extends DriverBase {
  private static Map<string, StorableObjectInterface> $data = Map {};
  private DriverConfigInterface $_config;

  public function __construct(DriverConfigInterface $config) {
    $this->_config = $config;
  }

  public function getConfig(): DriverConfigInterface {
    return $this->_config;
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
      $storableObject = self::$data->get($key);

      // no data to work with.
      if ($storableObject === null) {
        return null;
      }

      return $storableObject;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function set(StorableObjectInterface $obj): bool {

    try {

      $key = $this->getConfig()->createKeyFromStorableObject($obj);

      self::$data->set($key, $obj);
      $storableObject = self::$data->get($key);

      return $storableObject === null ? false : true;

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
      self::$data->remove($key);
      $success = !self::$data->contains($key);

      return $success;

    } catch (Exception $e) {
      throw $e;
    }
  }
}
