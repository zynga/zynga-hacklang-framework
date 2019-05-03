<?hh // strict

namespace Zynga\Framework\Factory\V2\Test\Config\Mock\ConfigThrowsException;

use Zynga\Framework\Factory\V2\Test\Config\Mock\Dev as ConfigBase;
use
  Zynga\Framework\Factory\V2\Test\Config\Mock\ConfigThrowsException\ConfigException
;

class Production extends ConfigBase {
  public function __construct() {
    throw new ConfigException('DEV_CONFIG_IS_BROKE');
  }
}
