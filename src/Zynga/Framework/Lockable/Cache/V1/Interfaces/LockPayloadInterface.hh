<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1\Interfaces;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

interface LockPayloadInterface extends StorableObjectInterface {
  public function setLockEstablishment(int $ts): bool;
  public function getLockEstablishment(): int;
  public function setLockRetryCount(int $count): bool;
  public function getLockRetryCount(): int;
  public function setBacktrace(string $backtrace): bool;
  public function getBacktrace(): string;
  public function isLockStillValid(int $lockTTL): bool;
}
