<?hh //strict

namespace Zynga\Framework\Factory\V2\Test\Config\Mock\ConfigThrowsException;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use
  Zynga\Framework\Factory\V2\Test\Config\Mock\ConfigThrowsException\Staging as TestConfig
;
use
  Zynga\Framework\Factory\V2\Test\Config\Mock\ConfigThrowsException\ConfigException
;

class StagingTest extends TestCase {

  public function test_configValues(): void {
    $this->expectException(ConfigException::class);
    $obj = new TestConfig();
  }

}
