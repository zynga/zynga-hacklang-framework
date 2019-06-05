<?hh // strict

namespace Zynga\Framework\Logging\V1C\onfig\Noop;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Logging\V1\Config\Noop\Dev as ConfigUnderTest;

class DevTest extends TestCase {
  public function testConfig(): void {
    $obj = new ConfigUnderTest();
    $this->assertTrue($obj->init());
    $this->assertEquals('Noop', $obj->getDriver());
  }
}
