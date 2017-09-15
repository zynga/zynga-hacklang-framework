<?hh // strict

namespace Zynga\Framework\Datadog\V2\Interfaces;

use Zynga\Framework\Factory\V2\Interfaces\ConfigInterface;

interface DriverConfigInterface extends ConfigInterface {
  public function getServerHostname(): string;
  public function getServerPort(): int;
  public function getDriver(): string;
}
