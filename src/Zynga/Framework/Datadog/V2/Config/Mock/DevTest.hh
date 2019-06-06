<?hh //strict

namespace Zynga\Framework\Datadog\V2\Config\Mock;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Datadog\V2\Config\Mock\Dev as ConfigUnderTest;

class DevTest extends TestCase {
  public function testConfig(): void {
    $obj = new ConfigUnderTest();
    $this->assertTrue($obj instanceof $obj);
    $this->assertEquals('', $obj->getServerHostname());
    $this->assertEquals(0, $obj->getServerPort());
  }
}
