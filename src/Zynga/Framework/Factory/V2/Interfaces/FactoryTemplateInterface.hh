<?hh // strict

namespace Zynga\Framework\Factory\V2\Interfaces;

use Zynga\Framework\Factory\V2\Interfaces\ConfigInterface;
use Zynga\Framework\Factory\V2\Interfaces\DriverInterface;

interface FactoryTemplateInterface {

  /**
   * Returns the current configuration value for the class path of your factory
   * implementation.
   * @return Vector<string>
   */
  public function getClassRoots(): Vector<string>;

  /**
   * Adds a namespace to hunt within for your configuration(s) or drivers(s).
   * @param string $root
   * @return bool
   */
  public function addClassRoot(string $root): bool;

  /**
   * Converts the requested name into a class name for use in dynamic inclusion.
   * @param  string $name
   * @return Vector<string>
   */
  public function createConfigClassNames(string $name): Vector<string>;

  /**
   * Converts the requested name into a class name for use in dynamic inclusion.
   * @param  string $name
   * @return Vector<string>
   */
  public function createDriverClassNames(
    ConfigInterface $config,
  ): Vector<string>;

  /**
   * Gets a database configuration for a given driver name.
   * @param  string $name
   * @return ConfigInterface
   */
  public function getConfigForName(string $name): ConfigInterface;

  /**
   * Gets a database configuration for a given driver name.
   * @param  string $name
   * @return DriverInterface
   */
  public function getDriverForName(string $name): DriverInterface;

  /**
   * Responsible for creating or returning driver for this factory template.
   * @param classname<TDriver> $driverName
   * @param string $name
   * @return DriverInterface
   */
  public function factory(string $name): DriverInterface;
  
  public function getDriverByClassname<TDriver as DriverInterface>(
    classname<TDriver> $driverName,
    string $configName,
  ): DriverInterface;

  /**
   * Clears all drivers from the factory.
   * @return bool
   */
  public function clear(): bool;

  /**
   * Returns the number of currently loaded drivers.
   * @return int
   */
  public function getDriverCount(): int;

  /**
   * Returns the current state of mock config override
   * @return bool
   */
  public function getUseMockDrivers(): bool;

  /**
   * Enables mock config override, causing all drivers returned to be your
   * config/Mock/... configuration
   * @return bool
   */
  public function enableMockDrivers(): bool;

  /**
   * Disables mock config override
   * @return bool
   */
  public function disableMockDrivers(): bool;

}
