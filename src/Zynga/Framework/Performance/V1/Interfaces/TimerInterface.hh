<?hh // strict

namespace Zynga\Framework\Performance\V1\Interfaces;

interface TimerInterface {

  public function recordStartTime(): bool;
  public function getStartTime(): float;

  public function recordStartMemory(): bool;
  public function getStartMemory(): int;

  public function recordEndTime(): bool;
  public function getEndTime(): float;

  public function recordEndMemory(): bool;
  public function getEndMemory(): int;

  public function getElapsedTime(): float;

  public function getTotalMemoryUsage(): int;

}
