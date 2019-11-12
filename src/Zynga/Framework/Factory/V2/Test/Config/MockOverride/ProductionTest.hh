<?hh //strict

namespace Zynga\Framework\Factory\V2\Test\Config\MockOverride;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use
  Zynga\Framework\Factory\V2\Test\Config\MockOverride\Production as TestConfig
;

class ProductionTest extends TestCase {
  public function test_configValues(): void {
    $obj = new TestConfig();
    $this->assertEquals(
      'This-is-overridden-Production',
      $obj->getExampleConfigValue(),
    );
  }
}
