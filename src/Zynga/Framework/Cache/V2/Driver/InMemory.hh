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

  public function add(
    StorableObjectInterface $obj,
    string $keyOverride = '',
    int $ttlOverride = -1,
  ): bool {

    try {

      $key = $this->getKeySupportingOverride($obj, $keyOverride);
      $ttl = $this->getTTLSupportingOverride($ttlOverride);

      $value = self::$data->get($key);

      // mimic the atomic lock of memcache, if there's a value it's already set.
      if ($value !== null) {
        return false;
      }

      self::$data->set($key, $obj);

      return true;

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

  public function set(
    StorableObjectInterface $obj,
    string $keyOverride = '',
    int $ttlOverride = -1,
  ): bool {

    try {

      $key = $this->getKeySupportingOverride($obj, $keyOverride);
      $ttl = $this->getTTLSupportingOverride($ttlOverride);

      self::$data->set($key, $obj);
      $storableObject = self::$data->get($key);

      return $storableObject === null ? false : true;

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

      $data = self::$data->get($key);

      if (!$data instanceof StorableObjectInterface) {
        return false;
      }

      self::$data->remove($key);

      return true;

    } catch (Exception $e) {
      throw $e;
    }
  }
  
  public function clearInMemoryCache(): bool {
    self::$data = Map {};
    return true;
  }
}
