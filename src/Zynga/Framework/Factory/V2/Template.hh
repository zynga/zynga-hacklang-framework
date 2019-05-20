<?hh // strict

namespace Zynga\Framework\Factory\V2;

use Zynga\Framework\Environment\DevelopmentMode\V1\DevelopmentMode;
use Zynga\Framework\Dynamic\V1\DynamicClassCreation;
use Zynga\Environment\V1\Exceptions\UnableToFindClassException;
use Zynga\Framework\Exception\V1\Exception;

use Zynga\Framework\Factory\V2\DriverContainer;
use Zynga\Framework\Factory\V2\Exceptions\FailedToLoadDriverConfigException;
use Zynga\Framework\Factory\V2\Exceptions\FailedToLoadDriverException;
use Zynga\Framework\Factory\V2\Interfaces\ConfigInterface;
use Zynga\Framework\Factory\V2\Interfaces\DriverContainerInterface;
use Zynga\Framework\Factory\V2\Interfaces\DriverInterface;
use Zynga\Framework\Factory\V2\Interfaces\FactoryTemplateInterface;

use \Exception as BaseException;

/**
 * Factory template class that handles the heavy lifting for standing up
 * instances following our standard layout:
 *
 * docs/FactoryFramework.md covers the topic area.
 *
 */
class Template implements FactoryTemplateInterface {

  private Vector<string> $_classRoots;

  private DriverContainerInterface<DriverInterface> $_drivers;
  private bool $_useMockDrivers;

  public function __construct(string $classRoot) {
    $this->_classRoots = Vector {$classRoot};
    $this->_drivers = new DriverContainer();
    $this->_useMockDrivers = false;
  }

  /**
   * Returns the current configuration value for the class path of your factory
   * implementation.
   * @return string
   */
  public function getClassRoots(): Vector<string> {
    return $this->_classRoots;
  }

  /**
   * Adds a namespace to hunt within for your configuration(s) or drivers(s).
   * @param string $root
   * @return bool
   */
  public function addClassRoot(string $root): bool {
    $this->_classRoots->add($root);
    return true;
  }

  /**
   * Converts the requested name into a class name for use in dynamic inclusion.
   * @param  string $name
   * @return string
   */
  public function createConfigClassNames(string $name): Vector<string> {

    $classNames = Vector {};

    // get the base path plus config path.
    $classRoots = $this->getClassRoots();

    foreach ($classRoots as $classRoot) {

      $path = $classRoot.'\\Config\\';

      // does this even have a underscore
      $pos = strpos($name, '_');

      DevelopmentMode::detect();

      $postFix = '\\'.DevelopmentMode::getModeAsString();

      if ($pos === false) {
        $path .= $name;
        $path .= $postFix;
        $classNames->add($path);
      } else {
        $path .= strtr($name, '_', '\\');
        $path .= $postFix;
        $classNames->add($path);
      }

    }

    return $classNames;

  }

  /**
   * Converts the requested name into a class name for use in dynamic inclusion.
   * @param  string $name
   * @return string
   */
  public function createDriverClassNames(
    ConfigInterface $config,
  ): Vector<string> {

    $classNames = Vector {};

    // get the base path plus driver path.
    $classRoots = $this->getClassRoots();

    foreach ($classRoots as $classRoot) {
      $path = $classRoot.'\\Driver\\'.$config->getDriver();
      $classNames->add($path);
    }

    return $classNames;

  }

  /**
   * Gets a database configuration for a given driver name.
   * @param  string $name
   * @return ConfigInterface
   */
  public function getConfigForName(string $name): ConfigInterface {

    $classNames = Vector {};

    try {

      $config = null;

      $classNames = $this->createConfigClassNames($name);

      foreach ($classNames as $className) {

        if (DynamicClassCreation::doesClassExist($className) === true) {

          $config =
            DynamicClassCreation::createClassByName($className, Vector {});

          // JEO: We have to take the mixed value and validate it as ConfigInterface
          invariant(
            $config instanceof ConfigInterface,
            '%s was not a ConfigInterface',
            $name,
          );

          return $config;

        }

      }

    } catch (BaseException $e) {

      $configException = new FailedToLoadDriverConfigException(
        'Failed to load driver for name='.$name.' message='.$e->getMessage(),
      );

      $configException->setPrevious($e);

      throw $configException;

    }

    throw new FailedToLoadDriverConfigException(
      'name='.$name.' '.'noClassesFoundWithin='.json_encode($classNames),
    );

  }

  /**
   * Gets a database configuration for a given driver name.
   * @param  string $name
   * @return DriverInterface
   */
  public function getDriverForName(string $name): DriverInterface {

    $classNames = Vector {};

    try {

      $config = $this->getConfigForName($name);

      $driver = null;

      $classNames = $this->createDriverClassNames($config);

      foreach ($classNames as $className) {

        if (DynamicClassCreation::doesClassExist($className) === true) {
          $driver = DynamicClassCreation::createClassByName(
            $className,
            Vector {$config},
          );

          invariant(
            $driver instanceof DriverInterface,
            'invariant=Failed to load driver for name=%s driver=%s',
            $name,
            $className,
          );

          return $driver;
        }

      }

    } catch (BaseException $e) {

      $failure = new FailedToLoadDriverException(
        'Failed to load driver for name='.$name.' message='.$e->getMessage(),
      );

      $failure->setPrevious($e);

      throw $failure;

    }

    throw new FailedToLoadDriverException(
      'name='.$name.' '.'classNames='.json_encode($classNames),
    );

  }

  /**
   * Responsible for creating or returning driver for this factory template.
   * @param string $name
   * @return DriverInterface
   */
  public function factory(string $name): DriverInterface {

    try {

      // if the driver is already stood up, return it.
      $driverContainer = $this->_drivers;

      $driver = null;

      if ($driverContainer->isDriverCached($name) === true) {
        return $driverContainer->getDriverFromCache($name);
      }

      // Try to stand up the driver shell
      $driver = $this->getDriverForName($name);

      // cache the driver
      $driverContainer->addDriverToCache($name, $driver);

      // return the driver
      return $driverContainer->getDriverFromCache($name);

    } catch (Exception $e) {
      throw $e;
    }

  }

  /**
   * Clears all drivers from the factory.
   * @return bool
   */
  public function clear(): bool {
    $driverContainer = $this->_drivers;
    return $driverContainer->clear();
  }

  /**
   * Returns the number of currently loaded drivers.
   * @return int
   */
  public function getDriverCount(): int {
    $driverContainer = $this->_drivers;
    return $driverContainer->count();
  }

  /**
   * Returns the current state of mock config override
   * @return bool
   */
  public function getUseMockDrivers(): bool {
    return $this->_useMockDrivers;
  }

  /**
   * Enables mock config override, causing all drivers returned to be your
   * config/Mock/... configuration
   * @return bool
   */
  public function enableMockDrivers(): bool {
    $this->_useMockDrivers = true;
    $this->clear();
    return true;
  }

  /**
   * Disables mock config override
   * @return bool
   */
  public function disableMockDrivers(): bool {
    $this->_useMockDrivers = false;
    $this->clear();
    return true;
  }

}
