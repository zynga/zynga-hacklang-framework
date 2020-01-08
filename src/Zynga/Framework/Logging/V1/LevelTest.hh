<?hh //strict

namespace Zynga\Framework\Logging\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Logging\V1\Level;

class LevelTest extends TestCase {

  public function test_levels(): void {
    $this->assertEquals(2, Level::DEBUG);
    $this->assertEquals(4, Level::INFO);
    $this->assertEquals(8, Level::WARNING);
    $this->assertEquals(16, Level::ERROR);
    $this->assertEquals(32, Level::CRITICAL);
  }

  public function test_getLevelAsString(): void {
    $this->assertEquals('DEBUG', Level::getLevelAsString(2));
    $this->assertEquals('INFO', Level::getLevelAsString(4));
    $this->assertEquals('WARNING', Level::getLevelAsString(8));
    $this->assertEquals('ERROR', Level::getLevelAsString(16));
    $this->assertEquals('CRITICAL', Level::getLevelAsString(32));
    $this->assertEquals('UNKNOWN', Level::getLevelAsString(0));
    $this->assertEquals('UNKNOWN', Level::getLevelAsString(-1));
  }

}
