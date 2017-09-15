<?hh //strict

namespace Zynga\Framework\Datadog\V2;

use Zynga\Framework\Datadog\V2\Factory as DatadogFactory;
use Zynga\Framework\Datadog\V2\Interfaces\DriverInterface as DatadogDriverInterface;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class MockTest extends TestCase {
  public function testServiceCheck(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Mock');
    $this->assertTrue($dog->serviceCheck('', 0));
  }
  public function testMockEvent(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Mock');
    $this->assertTrue($dog->event('not-a-real-stat', 'not-a-real-value'));
  }
}
