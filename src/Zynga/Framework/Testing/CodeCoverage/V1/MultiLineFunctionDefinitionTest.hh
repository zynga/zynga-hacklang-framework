<?hh // strict

namespace Zynga\Framework\Testing\CodeCoverage\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as ZyngaTestCase;
use Zynga\Framework\Testing\CodeCoverage\V1\MultiLineFunctionDefinition;

use \Exception;

class MultiLineFunctionDefinitionTest extends ZyngaTestCase {

  public function testDoBigThrow(): void {
    $obj = new MultiLineFunctionDefinition();
    $this->assertTrue($obj->doSomethingGreat('param1', 'param2', 'param3'));
  }

}
