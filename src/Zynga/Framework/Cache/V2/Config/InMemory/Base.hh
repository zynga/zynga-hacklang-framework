<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\InMemory;

use Zynga\Framework\Cache\V2\Config\Base as ConfigBase;

abstract class Base extends ConfigBase {

  public function getServerPairings(): Map<string, int> {
    return Map {};
  }

  public function getDriver(): string {
    return 'InMemory';
  }

  public function getTTL(): int {
    return 3600;
  }
}
