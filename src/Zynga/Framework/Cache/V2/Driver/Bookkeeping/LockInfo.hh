<?hh //strict

namespace Zynga\Framework\Cache\V2\Driver\Bookkeeping;

use Zynga\Framework\Cache\V2\Exceptions\InvalidLockCountException;

class LockInfo {
  private int $count;
  private int $timeoutInUnixTime;

  public function __construct(int $count, int $timeoutInUnixTime) {
    $this->count = $count;
    $this->timeoutInUnixTime = $timeoutInUnixTime;
  }

  public function hasTimedOut(int $timeInSeconds): bool {
    return $timeInSeconds >= $this->timeoutInUnixTime;
  }

  public function increment(): void {
    $this->count++;
  }

  public function decrement(): void {
    $this->count--;

    if ($this->count < 0) {
      throw new InvalidLockCountException(
        __METHOD__,
        'Lock should never be decremented below 0.',
        Map {'count' => $this->count},
      );
    }
  }

  public function hasNoLocks(): bool {
    return $this->count === 0;
  }

  public function getTimeoutInUnixTime(): int {
    return $this->timeoutInUnixTime;
  }
}
