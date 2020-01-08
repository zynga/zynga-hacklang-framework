<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1;

use Zynga\Framework\StorableObject\V1\Base;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\UInt64Box;
use Zynga\Framework\Lockable\Cache\V1\Interfaces\LockPayloadInterface;

class LockPayload extends Base implements LockPayloadInterface {
  public UInt64Box $lockEstablishment;
  public UInt64Box $lockRetryCount;
  public StringBox $backtrace;

  public function __construct() {

    $this->lockEstablishment = new UInt64Box();
    $this->lockEstablishment->setIsRequired(true);
    
    $this->lockRetryCount = new UInt64Box();
    $this->lockRetryCount->setDefaultValue(0);

    $this->backtrace = new StringBox();
    $this->backtrace->setIsRequired(true);

    parent::__construct();

  }

  public function setLockEstablishment(int $ts): bool {
    return $this->lockEstablishment->set($ts);
  }

  public function getLockEstablishment(): int {
    return $this->lockEstablishment->get();
  }
  
  public function setLockRetryCount(int $count): bool {
    return $this->lockRetryCount->set($count);
  }
  public function getLockRetryCount(): int {
    return $this->lockRetryCount->get();
  }

  public function setBacktrace(string $backtrace): bool {
    return $this->backtrace->set($backtrace);
  }

  public function getBacktrace(): string {
    return $this->backtrace->get();
  }

  public function isLockStillValid(int $lockTTL): bool {

    $establishment = $this->getLockEstablishment();
    $lockTimeout = $establishment + $lockTTL;
    $currentTime = time();

    if ($currentTime > $lockTimeout) {
      return false;
    }

    return true;

  }

}
