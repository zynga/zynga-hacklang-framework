<?hh // strict

namespace Zynga\Framework\Cache\V2\Interfaces;

interface MemcacheDriverInterface extends DriverInterface {
  public function directIncrement(string $key, int $incrementValue = 1): int;
    
  public function directAdd(
    string $key,
    mixed $value,
    int $flags = 0,
    int $ttl = 0,
  ): bool;
  
  public function directDelete(string $key): bool;
  
  public function connect(): bool;
}
