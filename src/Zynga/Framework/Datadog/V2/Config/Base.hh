<?hh // strict

namespace Zynga\Framework\Datadog\V2\Config;

use Zynga\Framework\Datadog\V2\Interfaces\DriverConfigInterface;

/**
 * Base Datadog driver config
 **/
abstract class Base implements DriverConfigInterface {

  final public function __construct() {
    $this->init();
  }

  /**
   * Initializes this instance of the driver configuration
   *
   * @return True if successful, otherwise false.
   **/
  public function init(): bool {
    return true;
  }
}
