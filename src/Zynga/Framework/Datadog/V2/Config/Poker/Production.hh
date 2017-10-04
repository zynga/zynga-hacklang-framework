<?hh // strict

namespace Zynga\Framework\Datadog\V2\Config\Poker;

use Zynga\Framework\Datadog\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Datadog\V2\Interfaces\DriverInterface;

class Production implements DriverConfigInterface {

  public function getServerHostname(): string {
    return '127.0.0.1';
  }

  public function getServerPort(): int {
    return 8125;
  }

  public function getDriver(): string {
    return 'UDP';
  }

}
