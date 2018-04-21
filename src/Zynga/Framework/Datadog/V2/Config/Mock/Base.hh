<?hh // strict

namespace Zynga\Framework\Datadog\V2\Config\Mock;

use Zynga\Framework\Datadog\V2\Config\Base as ConfigBase;

abstract class Base extends ConfigBase {
  public function getDriver(): string {
    return 'Mock';
  }

  public function getServerHostname(): string {
    // Because this is an abstract class, it will never be directly called
    return '';
  }

  public function getServerPort(): int {
    // Because this is an abstract class, it will never be directly called
    return 0;
  }
}
