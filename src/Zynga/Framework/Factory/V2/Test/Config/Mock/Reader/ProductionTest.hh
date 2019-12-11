<?hh //strict

namespace Zynga\Framework\Factory\V2\Test\Config\Mock\Reader;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use
  Zynga\Framework\Factory\V2\Test\Config\Mock\Reader\Production as TestConfig
;

class ProductionTest extends TestCase {
  public function test_configValues(): void {
    $obj = new TestConfig();
    $this->assertEquals(
      'This-is-Production-Reader',
      $obj->getExampleConfigValue(),
    );
  }
}
