<?hh // strict

namespace Zynga\Framework\Factory\V2\Test\Config\Mock\Reader;

use Zynga\Framework\Factory\V2\Test\Config\Mock\Base as ConfigBase;

class Staging extends ConfigBase {
  public function getExampleConfigValue(): string {
    return 'This-is-Staging-Reader';
  }
}
