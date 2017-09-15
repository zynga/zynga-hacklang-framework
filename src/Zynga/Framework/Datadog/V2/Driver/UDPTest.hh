<?hh //strict

namespace Zynga\Framework\Datadog\V2;

use Zynga\Framework\Datadog\V2\Driver\UDP;
use Zynga\Framework\Datadog\V2\Factory as DatadogFactory;
use Zynga\Framework\Datadog\V2\Interfaces\DriverInterface as DatadogDriverInterface;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class UDPTest extends TestCase {

  public function doSetUpBeforeClass(): bool {
    parent::doSetUpBeforeClass();
    DatadogFactory::disableMockDrivers();
    return true;
  }

  public function testShouldSample(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');
    $this->assertTrue($dog->shouldSample(0.0, true));
  }

  public function testSendForcedSampleWithTags(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');
    $this->assertTrue($dog instanceof UDP);
    if ($dog instanceof UDP) {
      $data = Map {};
      $data['a'] = 'b|ms';
      $tags = Map {};
      $tags['a'] = 'b';
      $this->assertTrue($dog->send($data, 0.5, $tags, true));
    }
  }

  public function testUpdateStat(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');
    $this->assertTrue($dog instanceof UDP);
    if ($dog instanceof UDP) {
      $this->assertTrue($dog->updateStat(''));
    }
  }

  public function testSEvent(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');
    $this->assertTrue($dog->event('testTitle', 'testText'));
  }

  public function testSetStats(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');
    $data = Map {};
    $data['a'] = 1.0;
    $this->assertTrue($dog->setStats($data));
  }

  public function testHistogramStats(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');
    $data = Map {};
    $data['a'] = 1.0;
    $this->assertTrue($dog->histogramStats($data));
  }

  public function testHistogram(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');
    $this->assertTrue($dog->histogram('a', 1.0));
  }

  public function testDecrementStats(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');
    $data = Vector {'testStat'};
    $this->assertTrue($dog->decrementStats($data, 1.0));
  }

  public function testDecrement(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');
    $this->assertTrue($dog->decrement('testStat', 1.0));
  }

  public function testIncrement(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');
    $this->assertTrue($dog->increment('testStat', 1.0));
  }

  public function testGauge(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');
    $this->assertTrue($dog->gauge('testStat', 1.0));
  }

  public function testTiming(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');
    $this->assertTrue($dog->timing('testStat', 1.0));
  }

  public function testCloseSocket(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');

    if ( $dog instanceof UDP ) {
      $this->assertTrue($dog->increment('testStat', 1.0));
      $this->assertTrue($dog->closeSocket());
      $this->assertFalse($dog->closeSocket());
    }

  }
}
