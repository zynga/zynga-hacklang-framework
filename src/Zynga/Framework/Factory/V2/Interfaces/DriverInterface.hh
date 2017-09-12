<?hh // strict

namespace Zynga\Framework\Factory\V2\Interfaces;

use Zynga\Framework\Factory\V2\Interfaces\ConfigInterface;

interface DriverInterface {
  public function getConfig(): ConfigInterface;
}
