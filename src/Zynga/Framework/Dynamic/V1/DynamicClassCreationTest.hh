<?hh // strict

namespace Zynga\Framework\Dynamic\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Dynamic\V1\DynamicClassCreation;
use Zynga\Framework\Dynamic\V1\Exceptions\UnableToFindClassException;
use Zynga\Framework\Dynamic\V1\Exceptions\MissingRequiredParametersException;
use Zynga\Framework\Dynamic\V1\Mocks\EmptyClass;
use Zynga\Framework\Dynamic\V1\Mocks\ClassWithConstructor;
use Zynga\Framework\Dynamic\V1\Mocks\ClassWithConstructorParams;

class DynamicClassCreationTest extends TestCase {

  public function test_DoesClassExist(): void {
    $this->assertTrue(DynamicClassCreation::doesClassExist('Zynga\Framework\Dynamic\V1\Mocks\EmptyClass'));
  }

  public function test_NegativeDoesClassExist(): void {
    $this->assertFalse(DynamicClassCreation::doesClassExist('Zynga\Framework\Dynamic\V1\Mocks\EmptyClassNeverExisted'));
  }

  public function test_ClassExists(): void {
    $obj = DynamicClassCreation::createClassByName('Zynga\Framework\Dynamic\V1\Mocks\EmptyClass', Vector {});
    $this->assertTrue($obj instanceof EmptyClass);
  }

  public function test_InvalidClass(): void {
    $this->expectException(UnableToFindClassException::class);
    $obj = DynamicClassCreation::createClassByName('Not_Real_Class', Vector {});
  }

  public function test_validWithConstructor(): void {
    $obj = DynamicClassCreation::createClassByName('Zynga\Framework\Dynamic\V1\Mocks\ClassWithConstructor', Vector {});
    $this->assertTrue($obj instanceof ClassWithConstructor);
  }

  public function test_validWithConstructorParams(): void {
    $obj = DynamicClassCreation::createClassByName('Zynga\Framework\Dynamic\V1\Mocks\ClassWithConstructorParams', Vector {"someString", 1234});
    $this->assertTrue($obj instanceof ClassWithConstructorParams);
  }

  public function test_validWithConstructorParamMisMatch(): void {
    $this->expectException(MissingRequiredParametersException::class);
    $obj = DynamicClassCreation::createClassByName('Zynga\Framework\Dynamic\V1\Mocks\ClassWithConstructorParams', Vector {"someString"});
  }

}
