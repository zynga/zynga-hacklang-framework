<?hh // strict

namespace Zynga\Framework\Factory\V2\Test\Config\Mock\Reader;

use Zynga\Framework\Factory\V2\Test\Config\Mock\Base as ConfigBase;

class Dev extends ConfigBase {
  public function getExampleConfigValue(): string {
    return 'This-is-Dev-Reader';
  }
}
