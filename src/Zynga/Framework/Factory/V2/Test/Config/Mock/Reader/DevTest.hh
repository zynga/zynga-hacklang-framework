<?hh //strict

namespace Zynga\Framework\Factory\V2\Test\Config\Mock\Reader;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Factory\V2\Test\Config\Mock\Reader\Dev as TestConfig;

class DevTest extends TestCase {
  public function test_configValues(): void {
    $obj = new TestConfig();
    $this->assertEquals('This-is-Dev-Reader', $obj->getExampleConfigValue());
  }
}
