<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\Mock\NoServersConfigured;

use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidExampleObject;

use Zynga\Exception;

class Production implements DriverConfigInterface {

  public function getServerPairings(): Map<string, int> {
    $hosts = Map {};
    return $hosts;
  }

  public function getDriver(): string {
    return 'Memcache';
  }

  public function getStorableObjectName(): string {
    return ValidExampleObject::class;
  }

  public function createKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {
    throw new Exception('not-valid-connection');
  }

  public function getTTL(): int {
    return 3600;
  }

}
