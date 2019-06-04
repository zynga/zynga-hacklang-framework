<?hh //strict

namespace Zynga\Framework\Database\V2\Config\Mock\Cluster\Broken;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use
  Zynga\Framework\Database\V2\Config\Mock\Cluster\Broken\Dev as ConfigUnderTest
;

use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;

class DevTest extends TestCase {

  /**
   * Test the constants for our config
   */
  public function testConstants(): void {
    $this->assertEquals('testusername', ConfigUnderTest::TEST_USERNAME);
    $this->assertEquals('testpassword', ConfigUnderTest::TEST_PASSWORD);
  }

}
