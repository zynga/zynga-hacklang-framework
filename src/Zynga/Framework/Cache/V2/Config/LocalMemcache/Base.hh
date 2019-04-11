<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\LocalMemcache;

use Zynga\Framework\Cache\V2\Config\Base as ConfigBase;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

abstract class Base extends ConfigBase {

  public function getServerPairings(): Map<string, int> {
    $hosts = Map {};
    $hosts['127.0.0.1'] = 11211;
    return $hosts;
  }

  public function getDriver(): string {
    return 'Memcache';
  }

  public function getTTL(): int {
    return 3600;
  }

  public function getLockFlags(StorableObjectInterface $obj): int {
    return 0;
  }

  public function getLockTTL(StorableObjectInterface $obj): int {
    return 1;
  }

  public function createLockPayloadFromStorableObject(
    StorableObjectInterface $obj,
  ): Map<string, string> {
    return Map {};
  }
}
