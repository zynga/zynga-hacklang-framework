<?hh // strict

namespace Zynga\Framework\Dynamic\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Dynamic\V1\DynamicClassCreation;
use Zynga\Framework\Dynamic\V1\Exceptions\UnableToFindClassException;
use Zynga\Framework\Dynamic\V1\Exceptions\MissingRequiredParametersException;
use Zynga\Framework\Dynamic\V1\Mocks\EmptyClass;
use Zynga\Framework\Dynamic\V1\Mocks\ClassWithConstructor;
use Zynga\Framework\Dynamic\V1\Mocks\ClassWithConstructorParams;
use Zynga\Framework\ReflectionCache\V1\ReflectionClasses;

class DynamicClassCreationTest extends TestCase {

  <<__Override>>
  public function tearDown(): void {
    parent::tearDown();

    // Disable failure for ReflectionClasses so that
    // it can function normally
    ReflectionClasses::disableFailure(); 
  }

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

  public function testFailureToReflectThrowsExceptionForClassExistsGeneric(
  ): void {
    $this->expectException(UnableToFindClassException::class);

    $className = 'Zynga\Framework\Dynamic\V1\Mocks\EmptyClass';
    ReflectionClasses::enableFailure($className);

    DynamicClassCreation::createClassByNameGeneric(
      $className,
      Vector {},
    );
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

  public function testFailureToReflectThrowsExceptionForDoesClassImplementInterface(
  ): void {
    $this->expectException(UnableToFindClassException::class);

    $className = 'Zynga\Framework\Dynamic\V1\Mocks\ClassWithSingleInterface';
    ReflectionClasses::enableFailure($className);

    DynamicClassCreation::doesClassImplementInterface(
      $className,
      'Zynga\Framework\Dynamic\V1\Mocks\EmptyInterface',
    );
  }

  public function testIsClassAbstract(): void {
    $this->assertTrue(
      DynamicClassCreation::isClassAbstract(
        'Zynga\Framework\Dynamic\V1\Mocks\AbstractClass',
      ),
      'AbstractClass is not marked as abstract',
    );
    $this->assertFalse(
      DynamicClassCreation::isClassAbstract(
        'Zynga\Framework\Dynamic\V1\Mocks\EmptyClass',
      ),
      'EmptyClass is marked as abstract',
    );
  }

  public function testInvalidClassThrowsExceptionForIsClassAbstract(): void {
    $this->expectException(UnableToFindClassException::class);
    $obj = DynamicClassCreation::isClassAbstract('Not_Real_Class');
  }

  public function testFailureToReflectThrowsExceptionForIsClassAbstract(
  ): void {
    $this->expectException(UnableToFindClassException::class);

    $className = 'Zynga\Framework\Dynamic\V1\Mocks\AbstractClass';
    ReflectionClasses::enableFailure($className);

    DynamicClassCreation::isClassAbstract($className);
  }
}
