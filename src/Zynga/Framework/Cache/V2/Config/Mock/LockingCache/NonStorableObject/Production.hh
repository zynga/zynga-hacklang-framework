<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\Mock\LockingCache\NonStorableObject;

use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidExampleObject;

use Zynga\Framework\Exception\V1\Exception;

class Production implements DriverConfigInterface {

  public function getServerPairings(): Map<string, int> {
    $hosts = Map {};
    $hosts['127.0.0.1'] = 11211;
    return $hosts;
  }

  public function getDriver(): string {
    return 'LockingMemcache';
  }

  public function getStorableObjectName(): string {
    return self::class;
  }

  public function createKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {
    throw new Exception('not-valid-connection');
  }

  public function getTTL(): int {
    return 3600;
  }

  public function createLockKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {
    if ($obj instanceof ValidExampleObject) {
      return 'lmc-lock-'.$obj->example_uint64->get();
    }

    throw new Exception('Unable to get generate key lock');
  }

  public function createLockPayloadFromStorableObject(
    StorableObjectInterface $obj,
  ): Map<string, string> {
    return Map {};
  }

  public function getLockFlags(StorableObjectInterface $obj): int {
    return 0;
  }

  public function getLockTTL(StorableObjectInterface $obj): int {
    return 1;
  }
}
