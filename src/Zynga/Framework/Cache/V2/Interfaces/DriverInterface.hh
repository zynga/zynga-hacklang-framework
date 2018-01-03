<?hh // strict

namespace Zynga\Framework\Cache\V2\Interfaces;

use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use
  Zynga\Framework\Factory\V2\Interfaces\DriverInterface as BaseDriverInterface
;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

interface DriverInterface extends BaseDriverInterface {
  public function getConfig(): DriverConfigInterface;
  public function getByMap(
    Map<string, mixed> $data,
  ): ?StorableObjectInterface;
  public function get(StorableObjectInterface $obj): ?StorableObjectInterface;
  public function set(StorableObjectInterface $obj): bool;
  public function deleteByMap(Map<string, mixed> $data): bool;
  public function delete(StorableObjectInterface $obj): bool;
}
