<?hh // strict

namespace Zynga\Framework\Cache\V2\Interfaces;

use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use
  Zynga\Framework\Factory\V2\Interfaces\DriverInterface as BaseDriverInterface
;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

interface DriverInterface extends BaseDriverInterface {

  /**
   *
   * Fetches the configuration for this driver
   *
   * @return DriverConfigInterface
   */
  public function getConfig(): DriverConfigInterface;

  /**
   *
   * Adds a storable object to a targeted key. Best effort per driver to make this a atomic operation.
   *
   * @param StorableObjectInterface $obj The object you want stored
   * @param string $keyOverride The string key you would like to store to in the case that config createStorableKey is needing to be overridden.
   * @return bool success
   */
  public function add(
    StorableObjectInterface $obj,
    string $keyOverride = '',
  ): bool;

  /**
   *
   * Attempts to fetch a storable from the cache, will return null if no such cached object exists.
   *
   * @param StorableObjectInterface $obj The object you want fetched.
   * @param string $keyOverride The string key you would like to store to in the case that config createStorableKey is needing to be overridden.
   * @return ?StorableObjectInterface the object or null.
   *
   */
  public function get(
    StorableObjectInterface $obj,
    string $keyOverride = '',
  ): ?StorableObjectInterface;

  /**
   *
   * Sets a storable object to a targeted key, will overwrite existing object this this object.
   *
   * @param StorableObjectInterface $obj The object you want stored
   * @param string $keyOverride The string key you would like to store to in the case that config createStorableKey is needing to be overridden.
   * @return bool success
   */
  public function set(
    StorableObjectInterface $obj,
    string $keyOverride = '',
  ): bool;

  /**
   *
   * Removes a storable object from a targeted key.
   *
   * @param StorableObjectInterface $obj The object you want stored
   * @param string $keyOverride The string key you would like to store to in the case that config createStorableKey is needing to be overridden.
   * @return bool success
   */
  public function delete(
    StorableObjectInterface $obj,
    string $keyOverride = '',
  ): bool;

}
