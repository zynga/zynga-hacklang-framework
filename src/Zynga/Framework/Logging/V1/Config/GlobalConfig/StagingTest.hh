<?hh // strict

namespace Zynga\Framework\Logging\V1C\onfig\GlobalConfig;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use
  Zynga\Framework\Logging\V1\Config\GlobalConfig\Staging as ConfigUnderTest
;

class StagingTest extends TestCase {
  public function testConfig(): void {
    $obj = new ConfigUnderTest();
    $this->assertTrue($obj->init());
    $this->assertEquals('Tee', $obj->getDriver());
  }
}
