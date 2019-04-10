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
 * Use this class when you want to mock memcache.
 */
class InMemory extends DriverBase {
  private static ?Map<string, StorableObjectInterface> $data;
  private DriverConfigInterface $_config;

  public function __construct(DriverConfigInterface $config) {
    $this->_config = $config;
    self::$data = Map {};
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

      $data = null;
      if (self::$data != null) {
        $data = self::$data->get($key);
      }

      // no data to work with.
      if ($data === null) {
        return null;
      }

      return $data;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function set(StorableObjectInterface $obj): bool {

    try {

      $key = $this->getConfig()->createKeyFromStorableObject($obj);

      $success = false;
      if (self::$data != null) {
        self::$data->set($key, $obj);
      }

      $returnedObject = null;
      if (self::$data != null) {
        $returnedObject = self::$data->get($key);
      }

      return $returnedObject === null ? false : true;

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

      $success = true;
      if (self::$data != null) {
        self::$data->remove($key);
      }

      if (self::$data != null) {
        $success = !self::$data->contains($key);
      }

      return $success;

    } catch (Exception $e) {
      throw $e;
    }
  }
}
