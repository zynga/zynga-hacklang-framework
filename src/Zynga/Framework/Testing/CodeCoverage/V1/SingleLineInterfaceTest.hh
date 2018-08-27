<?hh // strict

namespace Zynga\Framework\Testing\CodeCoverage\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as ZyngaTestCase;
use Zynga\Framework\Testing\CodeCoverage\V1\SingleLineInterface;
use
  Zynga\Framework\Testing\CodeCoverage\V1\ImplementsSingleLineInterfaceClass
;

use \Exception;

class SingleLineInterfaceTest extends ZyngaTestCase {

  public function testInterfaceUsage(): void {
    $this->assertEquals(
      'Zynga\Framework\Testing\CodeCoverage\V1\SingleLineInterface',
      SingleLineInterface::class,
    );
  }

  public function testImplementationUsage(): void {
    $obj = new ImplementsSingleLineInterfaceClass();
    $this->assertTrue($obj instanceof SingleLineInterface);
  }
}
