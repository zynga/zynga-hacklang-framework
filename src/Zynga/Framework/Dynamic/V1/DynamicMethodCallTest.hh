<?hh // strict

namespace Zynga\Framework\Dynamic\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Dynamic\V1\DynamicMethodCall;
use Zynga\Framework\Dynamic\V1\Exceptions\UnableToFindClassException;
use Zynga\Framework\Dynamic\V1\Exceptions\UnableToFindMethodException;
use Zynga\Framework\Dynamic\V1\Exceptions\MissingRequiredParametersException;

class DynamicMethodCallTest extends TestCase {

  public function test_ClassExists(): void {
    $ret = DynamicMethodCall::callMethod('Zynga\Framework\Dynamic\V1\Mocks\StaticClass', 'testFunction', Vector {});
    $this->assertTrue($ret === true);
  }

  public function test_InvalidClass(): void {
    $this->expectException(UnableToFindClassException::class);
    $ret = DynamicMethodCall::callMethod('Not_Real_Class', 'not_real_function', Vector {});
  }

  public function test_InvalidClass_SoftFailure(): void {
    $ret = DynamicMethodCall::callMethod('Not_Real_Class', 'not_real_function', Vector {}, true);
    $this->assertEquals(null, $ret);
  }

  public function test_ValidClassInvalidFunction(): void {
    $this->expectException(UnableToFindMethodException::class);
    $obj = DynamicMethodCall::callMethod('Zynga\Framework\Dynamic\V1\Mocks\StaticClass', 'not_real_function', Vector {});
  }

  public function test_validWithParams(): void {
    $ret = DynamicMethodCall::callMethod('Zynga\Framework\Dynamic\V1\Mocks\StaticClass', 'testFunctionWithParams', Vector {"someString", 1234});
    $this->assertEquals('someString:1234', $ret);
  }

  public function test_validWithParamsMismatch(): void {
    $this->expectException(MissingRequiredParametersException::class);
    $ret = DynamicMethodCall::callMethod('Zynga\Framework\Dynamic\V1\Mocks\StaticClass', 'testFunctionWithParams', Vector {"someString"});
  }
}
