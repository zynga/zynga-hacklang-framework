<?hh // strict

namespace Zynga\Framework\Performance\V1\Interfaces;

use Zynga\Framework\Performance\V1\Interfaces\TimerInterface;

interface PerformanceInterface {
  public function getTimer(string $timerName): TimerInterface;
  public function removeTimer(string $timerName): bool;
  public function startTimer(string $timerName): bool;
  public function endTimer(string $timerName): bool;
  public function getAllElapsedValues(): Map<string, float>;
  public function getAllTotalMemoryUsageValues(): Map<string, int>;
}
