<?hh // strict

namespace Zynga\Framework\Factory\V2\Test\Config\Mock;

use Zynga\Framework\Factory\V2\Config\Base as ConfigBase;
use Zynga\Framework\Factory\V2\Test\Interfaces\ConfigInterface;

abstract class Base extends ConfigBase implements ConfigInterface {
  public function getDriver(): string {
    return 'Mock';
  }
}
