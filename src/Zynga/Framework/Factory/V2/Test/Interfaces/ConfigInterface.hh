<?hh // strict

namespace Zynga\Framework\Factory\V2\Test\Interfaces;

use
  Zynga\Framework\Factory\V2\Interfaces\ConfigInterface as BaseConfigInterface
;

interface ConfigInterface extends BaseConfigInterface {
  public function getExampleConfigValue(): string;
}
