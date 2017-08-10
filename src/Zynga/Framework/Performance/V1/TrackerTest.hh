<?hh //strict

namespace Zynga\Environment\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Performance\V1\Tracker as PerformanceTracker;

class PerformanceTest extends TestCase {
  public function testOK(): void {

    $timerName = 'a-timer';
    $sleepTime = 100000;

    $perf = new PerformanceTracker();

    $this->assertTrue($perf->startTimer($timerName));
    $this->assertFalse($perf->startTimer($timerName));

    // sleep the dragon.
    usleep($sleepTime);

    $this->assertTrue($perf->endTimer($timerName));
    $this->assertFalse($perf->endTimer($timerName));

    $timer = $perf->getTimer($timerName);

    // is the start and end ints and are they the correct precendence.
    $this->assertTrue(is_float($timer->getStartTime()));
    $this->assertTrue(is_float($timer->getEndTime()));
    $this->assertTrue($timer->getStartTime() > 0.0);
    $this->assertTrue($timer->getEndTime() > 0.0);
    $this->assertTrue($timer->getStartTime() < $timer->getEndTime());

    // we shoudl be able to dig through this and see the data.
    $this->assertTrue(is_int($timer->getStartMemory()));
    $this->assertTrue(is_int($timer->getEndMemory()));
    $this->assertTrue($timer->getStartMemory() > 0);
    $this->assertTrue($timer->getEndMemory() > 0);
    $this->assertTrue($timer->getEndMemory() > $timer->getStartMemory());

    $timeMap = $perf->getAllElapsedValues();

    //var_dump($timer);
    //var_dump($timeMap[$timerName]);
    //var_dump($sleepTime / 1000000);
    $biggerInTime = $timeMap[$timerName] >= ($sleepTime / 1000000);
    //var_dump($biggerInTime);

    $this->assertTrue(is_float($timeMap[$timerName]));
    $this->assertTrue($biggerInTime);

    $memMap = $perf->getAllTotalMemoryUsageValues();

    $this->assertTrue(is_int($memMap[$timerName]));
    $this->assertTrue($memMap[$timerName] > 0);

    $this->assertTrue($perf->removeTimer($timerName));
    $this->assertFalse($perf->removeTimer($timerName));

  }

  public function testTimerEdges(): void {

    $timerName = 'a-timer';

    $perf = new PerformanceTracker();
    $timer = $perf->getTimer($timerName);

    $this->assertFalse($timer->recordEndTime());
    $this->assertEquals(0.0, $timer->getElapsedTime());
    $this->assertEquals(0, $timer->getTotalMemoryUsage());

  }

}
