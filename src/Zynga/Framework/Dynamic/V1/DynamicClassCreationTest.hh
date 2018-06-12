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

  public function testDoesClassExist(): void {
    $this->assertTrue(
      DynamicClassCreation::doesClassExist(
        'Zynga\Framework\Dynamic\V1\Mocks\EmptyClass',
      ),
    );
  }

  public function testNegativeDoesClassExist(): void {
    $this->assertFalse(
      DynamicClassCreation::doesClassExist(
        'Zynga\Framework\Dynamic\V1\Mocks\EmptyClassNeverExisted',
      ),
    );
  }

  public function testClassExistsGeneric(): void {
    $obj = DynamicClassCreation::createClassByNameGeneric(
      'Zynga\Framework\Dynamic\V1\Mocks\EmptyClass',
      Vector {},
    );
    $this->assertTrue($obj instanceof EmptyClass);
  }

  public function testClassExists(): void {
    $obj = DynamicClassCreation::createClassByName(
      'Zynga\Framework\Dynamic\V1\Mocks\EmptyClass',
      Vector {},
    );
    $this->assertTrue($obj instanceof EmptyClass);
  }

  public function testInvalidClass(): void {
    $this->expectException(UnableToFindClassException::class);
    $obj =
      DynamicClassCreation::createClassByName('Not_Real_Class', Vector {});
  }

  public function testValidWithConstructor(): void {
    $obj = DynamicClassCreation::createClassByName(
      'Zynga\Framework\Dynamic\V1\Mocks\ClassWithConstructor',
      Vector {},
    );
    $this->assertTrue($obj instanceof ClassWithConstructor);
  }

  public function testValidWithConstructorParams(): void {
    $obj = DynamicClassCreation::createClassByName(
      'Zynga\Framework\Dynamic\V1\Mocks\ClassWithConstructorParams',
      Vector {"someString", 1234},
    );
    $this->assertTrue($obj instanceof ClassWithConstructorParams);
  }

  public function testValidWithConstructorParamMisMatch(): void {
    $this->expectException(MissingRequiredParametersException::class);
    $obj = DynamicClassCreation::createClassByName(
      'Zynga\Framework\Dynamic\V1\Mocks\ClassWithConstructorParams',
      Vector {"someString"},
    );
  }

  public function testInvalidClassThrowsExceptionFromDoesClassImplementInterface(
  ): void {
    $this->expectException(UnableToFindClassException::class);
    DynamicClassCreation::doesClassImplementInterface(
      'Not_Real_Class',
      'Zynga\Framework\Dynamic\V1\Mocks\EmptyInterface',
    );
  }

  public function testClassWithSingleInterfaceExtendsOnlyItsInterface(): void {
    $this->assertTrue(
      DynamicClassCreation::doesClassImplementInterface(
        'Zynga\Framework\Dynamic\V1\Mocks\ClassWithSingleInterface',
        'Zynga\Framework\Dynamic\V1\Mocks\EmptyInterface',
      ),
      'ClassWithSingleInterface should implement EmptyInterface',
    );
    $this->assertFalse(
      DynamicClassCreation::doesClassImplementInterface(
        'Zynga\Framework\Dynamic\V1\Mocks\ClassWithSingleInterface',
        'Zynga\Framework\Dynamic\V1\Mocks\EmptyInterfaceTwo',
      ),
      'ClassWithSingleInterface should not implement EmptyInterfaceTwo',
    );
  }

  public function testClassWithMultipleInterfacesExtendsOnlyItsInterfaces(
  ): void {
    $this->assertTrue(
      DynamicClassCreation::doesClassImplementInterface(
        'Zynga\Framework\Dynamic\V1\Mocks\ClassWithMultipleInterfaces',
        'Zynga\Framework\Dynamic\V1\Mocks\EmptyInterface',
      ),
      'ClassWithMultipleInterfaces should implement EmptyInterface',
    );
    $this->assertTrue(
      DynamicClassCreation::doesClassImplementInterface(
        'Zynga\Framework\Dynamic\V1\Mocks\ClassWithMultipleInterfaces',
        'Zynga\Framework\Dynamic\V1\Mocks\EmptyInterfaceTwo',
      ),
      'ClassWithMultipleInterfaces should implement EmptyInterface',
    );
    $this->assertFalse(
      DynamicClassCreation::doesClassImplementInterface(
        'Zynga\Framework\Dynamic\V1\Mocks\ClassWithMultipleInterfaces',
        'Zynga\Framework\Dynamic\V1\Mocks\EmptyInterfaceThree',
      ),
      'ClassWithMultipleInterfaces should not implement EmptyInterfaceThree',
    );
  }

}
