<?hh // strict

namespace Zynga\Framework\Factory\V2\Test\Config\MockOverride;

use Zynga\Framework\Factory\V2\Test\Config\MockOverride\Base as ConfigBase;

class Staging extends ConfigBase {
  public function getExampleConfigValue(): string {
    return 'This-is-overridden-Staging';
  }
}
