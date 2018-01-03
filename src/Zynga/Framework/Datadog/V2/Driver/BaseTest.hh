<?hh //strict

namespace Zynga\Framework\Datadog\V2;

use Zynga\Framework\Datadog\V2\Factory as DatadogFactory;
use
  Zynga\Framework\Datadog\V2\Interfaces\DriverInterface as DatadogDriverInterface
;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class BaseTest extends TestCase {
  public function testSetRequireMockQueries(): void {
    $dog = DatadogFactory::factory(DatadogDriverInterface::class, 'Poker');
    $dog->setRequireMockQueries(false);
    $this->assertFalse($dog->getRequireMockQueries());
    $dog->setRequireMockQueries(true);
    $this->assertTrue($dog->getRequireMockQueries());
  }
}
