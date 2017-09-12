<?hh // strict

namespace Zynga\Framework\Factory\V2\Test\Config\Mock\ConfigThrowsException;

use Zynga\Framework\Factory\V2\Test\Config\Mock\Dev as ConfigBase;
use Zynga\Framework\Exception\V1\Exception;

class Dev extends ConfigBase {
  public function __construct() {
    throw new Exception('DEV_CONFIG_IS_BROKE');
  }
}
