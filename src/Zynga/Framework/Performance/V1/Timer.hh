<?hh // strict

namespace Zynga\Framework\Performance\V1;

use Zynga\Framework\Performance\V1\Interfaces\TimerInterface;

class Timer implements TimerInterface {

  private float $_startTime;
  private float $_endTime;

  private int $_startMemory;
  private int $_endMemory;

  public function __construct() {

    $this->_startTime = 0.0;
    $this->_endTime = 0.0;

    $this->_startMemory = 0;
    $this->_endMemory = 0;

  }

  public function recordStartTime(): bool {
    $this->_startTime = microtime(true);
    return true;
  }

  public function getStartTime(): float {
    return $this->_startTime;
  }

  public function recordEndTime(): bool {

    $timeStamp = microtime(true);

    // we can't end before we start.
    if ($this->_startTime != 0.0 && $timeStamp > $this->_startTime) {
      $this->_endTime = $timeStamp;
      return true;
    }

    return false;
  }

  public function getEndTime(): float {
    return $this->_endTime;
  }

  public function recordStartMemory(): bool {
    $this->_startMemory = memory_get_usage(true);
    return true;
  }

  public function getStartMemory(): int {
    return $this->_startMemory;
  }

  public function recordEndMemory(): bool {
    $this->_endMemory = memory_get_usage(true);
    return true;
  }

  public function getEndMemory(): int {
    return $this->_endMemory;
  }

  public function getElapsedTime(): float {

    if ($this->_endTime == 0.0) {
      return 0.0;
    }

    return $this->_endTime - $this->_startTime;

  }

  public function getTotalMemoryUsage(): int {

    if ($this->_endTime == 0.0) {
      return 0;
    }

    return $this->_endMemory - $this->_startMemory;

  }

}
