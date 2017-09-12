<?hh // strict

namespace Zynga\Framework\Factory\V2;

use
  Zynga\Framework\Factory\V2\Exceptions\DriverContainer\DriverAlreadyInCacheException
;
use Zynga\Framework\Factory\V2\Exceptions\DriverContainer\DriverNotInCacheException;
use Zynga\Framework\Factory\V2\Interfaces\DriverContainerInterface;
use Zynga\Framework\Factory\V2\Interfaces\DriverInterface;

class DriverContainer<TDriver> implements DriverContainerInterface<TDriver> {

  /**
   * @param Map<string, T> Cache for the database drivers that are currently alive.
   */
  private Map<string, TDriver> $_drivers;

  public function __construct() {
    $this->_drivers = Map {};
  }

  public function isDriverCached(string $name): bool {
    if ($this->_drivers->containsKey($name) === true) {
      return true;
    }
    return false;
  }

  public function getDriverFromCache(string $name): TDriver {
    if ($this->isDriverCached($name)) {
      return $this->_drivers[$name];
    }
    throw new DriverNotInCacheException('FAILED driver is not within cache');
  }

  public function addDriverToCache(string $name, TDriver $driver): bool {
    if ($this->isDriverCached($name) !== true) {
      $this->_drivers[$name] = $driver;
      return true;
    }
    throw new DriverAlreadyInCacheException(
      'FAILED driver is already within cache',
    );
  }

  public function removeDriverFromCache(string $name): bool {
    if ($this->isDriverCached($name) === true) {
      $this->_drivers->removeKey($name);
      return true;
    }
    throw new DriverNotInCacheException('FAILED driver is not within cache');
  }

  public function count(): int {
    return $this->_drivers->count();
  }

  public function clear(): bool {
    $this->_drivers->clear();
    return true;
  }

}
