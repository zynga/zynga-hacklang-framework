<?hh // strict

namespace Zynga\Framework\Performance\V1;

use Zynga\Framework\Performance\V1\Interfaces\PerformanceInterface;
use Zynga\Framework\Performance\V1\Interfaces\TimerInterface;
use Zynga\Framework\Performance\V1\Timer as PerfTimer;

class Tracker implements PerformanceInterface {

  private Map<string, TimerInterface> $_timers;

  public function __construct() {
    $this->_timers = Map {};
  }

  public function getTimer(string $timerName): TimerInterface {

    if (!$this->_timers->containsKey($timerName)) {
      $this->_timers[$timerName] = new PerfTimer();
    }

    return $this->_timers[$timerName];

  }

  public function removeTimer(string $timerName): bool {
    if ($this->_timers->contains($timerName)) {
      $this->_timers->removeKey($timerName);
      return true;
    }
    return false;
  }

  public function startTimer(string $timerName): bool {

    $timer = $this->getTimer($timerName);

    if ($timer->getStartTime() != 0) {
      return false;
    }

    $timer->recordStartTime();
    $timer->recordStartMemory();

    return true;
  }

  public function endTimer(string $timerName): bool {

    $timer = $this->getTimer($timerName);

    if ($timer->getEndTime() != 0) {
      return false;
    }

    $timer->recordEndTime();
    $timer->recordEndMemory();

    return true;

  }

  public function getAllElapsedValues(): Map<string, float> {
    $timerValues = Map {};
    foreach ($this->_timers as $timerName => $timer) {
      $timerValues[$timerName] = $timer->getElapsedTime();
    }
    return $timerValues;
  }

  public function getAllTotalMemoryUsageValues(): Map<string, int> {
    $timerValues = Map {};
    foreach ($this->_timers as $timerName => $timer) {
      $timerValues[$timerName] = $timer->getTotalMemoryUsage();
    }
    return $timerValues;
  }

}
