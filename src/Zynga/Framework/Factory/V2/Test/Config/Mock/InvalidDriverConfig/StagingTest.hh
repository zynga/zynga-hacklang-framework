<?hh //strict

namespace Zynga\Framework\Factory\V2\Test\Config\Mock\InvalidDriver;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use
  Zynga\Framework\Factory\V2\Test\Config\Mock\InvalidDriverConfig\Staging as TestConfig
;

class StagingTest extends TestCase {

  public function test_configValues(): void {
    $obj = new TestConfig();
    $this->assertEquals('InvalidDriverConfig', $obj->getDriver());
    $this->assertEquals('This-is-Staging', $obj->getExampleConfigValue());
  }

}
