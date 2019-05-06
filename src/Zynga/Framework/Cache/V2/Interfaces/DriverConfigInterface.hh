<?hh // strict

namespace Zynga\Framework\Cache\V2\Interfaces;

use Zynga\Framework\Factory\V2\Interfaces\ConfigInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

interface DriverConfigInterface extends ConfigInterface {
  public function getServerPairings(): Map<string, int>;
  public function getDriver(): string;
  public function createKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string;
  public function getTTL(): int;
}
