<?hh //strict

namespace Zynga\Framework\StorableObject\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\StorableObject\V1\Test\Mock\Valid;
use Zynga\Framework\StorableObject\V1\Test\Mock\Broken\ValidButBrokenFields;
use Zynga\Framework\StorableObject\V1\Test\Mock\InvalidUnsupportedType;
use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldGeneric;
use Zynga\Framework\StorableObject\V1\Exceptions\InvalidObjectException;
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;
use Zynga\Framework\StorableObject\V1\StorableMap;
use
  Zynga\Framework\StorableObject\Collections\Vector\V1\Base as StorableVector
;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\ReflectionCache\V1\ReflectionClasses;

use \ReflectionClass;

class GenericTest extends TestCase {
  private StorableMap<StringBox> $_testStorableMap;
  private StorableVector<StringBox> $_testStorableVector;

  public function __construct(string $name) {

    parent::__construct($name);

    $this->_testStorableMap = new StorableMap();
    $this->_testStorableVector = new StorableVector(StringBox::class);

  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\InvalidObjectException
   */
  public function testGetForObject_InvalidObject(): void {
    $nonObject = 'ThisIsNotAnObject';
    FieldGeneric::getForObject($nonObject);
  }

  public function testGetForObject_Valid(): void {
    $obj = new Valid();

    $map = FieldGeneric::getForObject($obj);

    $this->assertEquals(3, $map->keys()->count());

  }

  public function testGetShortNameForTypeBoxName(): void {

    $this->assertEquals(
      StringBox::class,
      FieldGeneric::getShortNameForTypeBoxName(
        'Zynga\Framework\Type\V1\StringBox',
      ),
    );

    $testReflection = ReflectionClasses::getReflection($this);

    if (!$testReflection instanceof ReflectionClass) {
      $this->assertTrue(
        false,
        'testUnableToBeReflected name='.get_class($this),
      );
      return;
    }

    $mapClass =
      $testReflection->getProperty('_testStorableMap')->getTypeText();
    $vecClass =
      $testReflection->getProperty('_testStorableVector')->getTypeText();

    $mapBoxType = FieldGeneric::getShortNameForTypeBoxName($mapClass);
    $this->assertEquals('StorableMap<'.StringBox::class.'>', $mapBoxType);
    // $this->assertEquals(StorableMap::class . '<'.StringBox::class.'>', $mapBoxType);

    $vecBoxType = FieldGeneric::getShortNameForTypeBoxName($vecClass);
    $this->assertEquals(
      StorableVector::class.'<'.StringBox::class.'>',
      $vecBoxType,
    );

  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException
   */
  // JEO: This test is dead, as I added support in many other places to enable us to
  // properly handle this via native types.
  /*
   public function testGetShortNameForTypeBoxName_Unsupported(): void {
   FieldGeneric::getShortNameForTypeBoxName('SomeUnknownTypeString');
   }
   */

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\InvalidObjectException
   */
  public function testGetFieldsAndTypesForObject_Unsupported(): void {
    FieldGeneric::getFieldsAndTypesForObject('SomeUnknownTypeString');
  }

  public function testGetFieldsAndTypesForObject(): void {

    $obj = new Valid();

    $fieldsAndTypes = FieldGeneric::getFieldsAndTypesForObject($obj);

    $this->assertEquals(3, $fieldsAndTypes->keys()->count());

  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\InvalidObjectException
   */
  public function testGetIsRequiredAndIsDefaultValue_Unsupported(): void {
    FieldGeneric::getIsRequiredAndIsDefaultValue('SomeUnknownTypeString');
  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException
   */
  public function testGetIsRequiredAndIsDefaultValue_UnsupportedType(): void {
    FieldGeneric::getIsRequiredAndIsDefaultValue($this);
  }

  public function testGetIsRequiredAndIsDefaultValue_Type(): void {

    $stringBox = new StringBox();

    list($isRequired, $isDefaultValue) =
      FieldGeneric::getIsRequiredAndIsDefaultValue($stringBox);
    $this->assertFalse($isRequired);
    $this->assertTrue($isDefaultValue);

    $stringBox->setIsRequired(true);

    list($isRequired, $isDefaultValue) =
      FieldGeneric::getIsRequiredAndIsDefaultValue($stringBox);
    $this->assertTrue($isRequired);
    $this->assertTrue($isDefaultValue);

    $stringBox->set('some-value');

    list($isRequired, $isDefaultValue) =
      FieldGeneric::getIsRequiredAndIsDefaultValue($stringBox);
    $this->assertTrue($isRequired);
    $this->assertFalse($isDefaultValue);

  }

  public function testGetIsRequiredAndIsDefaultValue_StorableObject(): void {

    $obj = new Valid();

    list($isRequired, $isDefaultValue) =
      FieldGeneric::getIsRequiredAndIsDefaultValue($obj);
    $this->assertFalse($isRequired);
    $this->assertTrue($isDefaultValue);

    $obj->setIsRequired(true);

    list($isRequired, $isDefaultValue) =
      FieldGeneric::getIsRequiredAndIsDefaultValue($obj);
    $this->assertTrue($isRequired);
    $this->assertTrue($isDefaultValue);

    $obj->example_float->set(12745.34);

    list($isRequired, $isDefaultValue) =
      FieldGeneric::getIsRequiredAndIsDefaultValue($obj);

    $this->assertTrue($isRequired);
    $this->assertFalse($isDefaultValue);

    $obj->example_string->set('some-value-string');

    list($isRequired, $isDefaultValue) =
      FieldGeneric::getIsRequiredAndIsDefaultValue($obj);
    $this->assertTrue($isRequired);
    $this->assertFalse($isDefaultValue);

  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function test_broken_fields_getForObject(): void {
    $obj = new ValidButBrokenFields();
    $obj->fields()->getForObject();
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function test_broken_fields_getTypedField(): void {
    $obj = new ValidButBrokenFields();
    $obj->fields()->getTypedField('field');
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function test_broken_fields_getFieldsAndTypesForObject(): void {
    $obj = new ValidButBrokenFields();
    $obj->fields()->getFieldsAndTypesForObject();
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function test_broken_fields_getRequiredFieldsWithDefaultValues(
  ): void {
    $obj = new ValidButBrokenFields();
    $obj->fields()->getRequiredFieldsWithDefaultValues('');
  }

  public function test_getTypedField_invalidObj(): void {
    $this->expectException(InvalidObjectException::class);
    FieldGeneric::getTypedField(null, 'testField');
  }

  public function test_getTypedField_missedField(): void {
    $obj = new Valid();
    $this->expectException(NoFieldsFoundException::class);
    FieldGeneric::getTypedField($obj, 'testFieldNotFound');
  }

  public function test_getTypedField_unsupportedType(): void {
    $obj = new InvalidUnsupportedType();
    $this->expectException(UnsupportedTypeException::class);
    FieldGeneric::getTypedField($obj, 'example_nonsupported');
  }

}
