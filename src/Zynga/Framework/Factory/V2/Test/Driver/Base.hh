<?hh // strict

namespace Zynga\Framework\Factory\V2\Test\Driver;

use Zynga\Framework\Factory\V2\Driver\Base as DriverBase;
use Zynga\Framework\Factory\V2\Test\Interfaces\ConfigInterface;
use Zynga\Framework\Factory\V2\Test\Interfaces\DriverInterface;

abstract class Base extends DriverBase implements DriverInterface {
  private ConfigInterface $_config;

  public function __construct(ConfigInterface $config) {
    $this->_config = $config;
  }

  public function getConfig(): ConfigInterface {
    return $this->_config;
  }

}
