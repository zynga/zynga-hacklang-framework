<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\Mock\BadConnection;

use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Cache\V2\Exceptions\InvalidObjectForKeyCreationException;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidExampleObject
;

class Production implements DriverConfigInterface {

  public function getServerPairings(): Map<string, int> {
    $hosts = Map {};
    // some rando ip
    $hosts['172.1.1.1'] = 11211;
    return $hosts;
  }

  public function getDriver(): string {
    return 'Memcache';
  }

  public function createKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {
    throw new InvalidObjectForKeyCreationException('not-valid-connection');
  }

  public function getTTL(): int {
    return 3600;
  }

}
