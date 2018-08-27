<?hh // strict

namespace Zynga\Framework\Testing\CodeCoverage\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as ZyngaTestCase;
use Zynga\Framework\Testing\CodeCoverage\V1\MultiLineThrow;

use \Exception;

class MultiLineThrowTest extends ZyngaTestCase {

  public function testDoBigThrow(): void {
    $this->expectException(Exception::class);
    $obj = new MultiLineThrow();
    $obj->doBigThrow();
  }

}
