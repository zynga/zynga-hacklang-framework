<?hh // strict

namespace Zynga\Framework\Factory\V2\Test\Config\Mock;

use Zynga\Framework\Factory\V2\Test\Config\Mock\Base as ConfigBase;

class Production extends ConfigBase {
  public function getExampleConfigValue(): string {
    return 'This-is-Production';
  }
}
