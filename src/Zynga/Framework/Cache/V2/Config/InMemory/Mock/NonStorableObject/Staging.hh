<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\InMemory\Mock\NonStorableObject;

use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;

use Zynga\Framework\Factory\V2\Config\Base as FactoryBaseConfig;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidExampleObject;

use Zynga\Framework\Exception\V1\Exception;

class Staging implements DriverConfigInterface {

  public function getServerPairings(): Map<string, int> {
    return Map {};
  }

  public function getDriver(): string {
    return 'InMemory';
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
}
