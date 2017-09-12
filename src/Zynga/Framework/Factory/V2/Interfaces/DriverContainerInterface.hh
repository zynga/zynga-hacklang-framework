<?hh // strict

namespace Zynga\Framework\Factory\V2\Interfaces;

use Zynga\Framework\Factory\V2\Interfaces\DriverInterface;

interface DriverContainerInterface<TDriver> {
  public function isDriverCached(string $name): bool;
  public function getDriverFromCache(string $name): TDriver;
  public function addDriverToCache(string $name, TDriver $driver): bool;
  public function removeDriverFromCache(string $name): bool;
  public function count(): int;
  public function clear(): bool;
}
