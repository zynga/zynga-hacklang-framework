<?hh //strict

namespace Zynga\Framework\StorableObject\Collections\Vector\V1;

use \OutOfBoundsException;
use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;

use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\UnsupportedCollectionValueType
;
use Zynga\Framework\StorableObject\Collections\Vector\V1\Base as VectorBase;
use
  Zynga\Framework\StorableObject\Collections\Vector\V1\Importer\Storable as StorableImporter
;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidStorableObject;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;

class BaseTest extends TestCase {

  public function testConstructionWithBoxSucceeds(): void {
    $collection = new VectorBase(ValidStorableObject::class);
    $this->addToAssertionCount(1);
  }

  public function testConstructionFailsForUnsupportedType(): void {
    $this->expectException(UnsupportedTypeException::class);
    $collection = new VectorBase(UnsupportedCollectionValueType::class);
  }

  public function testIsEmptyReturnsTrueWhenCollectionIsEmpty(): void {
    $collection = new VectorBase(ValidStorableObject::class);
    $this->assertTrue($collection->isEmpty());
  }

  public function testIsEmptyReturnsFalseWhenCollectionIsNotEmpty(): void {
    $collection = new VectorBase(ValidStorableObject::class);
    $collection->add(new ValidStorableObject());

    $this->assertFalse($collection->isEmpty());
  }

  public function testCountReturnsExpectedValue(): void {
    $collection = new VectorBase(ValidStorableObject::class);
    $collection->add(new ValidStorableObject());

    $this->assertEquals(1, $collection->count());

    $collection->add(new ValidStorableObject());

    $this->assertEquals(2, $collection->count());
  }

  public function testIteratorWithAllItemsIsReturned(): void {
    $collection = new VectorBase(ValidStorableObject::class);

    $collection->add(new ValidStorableObject())
      ->add(new ValidStorableObject())
      ->add(new ValidStorableObject());

    $items = $collection->items();

    $cur = 0;
    foreach ($items as $entry) {
      $cur++;
    }

    $this->assertEquals(3, $cur);
  }

  public function testClearRemovesAllItems(): void {
    $collection = new VectorBase(ValidStorableObject::class);

    $collection->add(new ValidStorableObject());
    $collection->clear();

    $this->assertEquals(0, $collection->count());
  }

  public function testAddAllInsertsItemsFromArray(): void {
    $collection = new VectorBase(ValidStorableObject::class);
    $array = array(new ValidStorableObject(), new ValidStorableObject());
    $collection->addAll($array);
    $this->assertEquals(2, $collection->count());
  }

  public function testAtReturnsExpectedValue(): void {
    $collection = new VectorBase(ValidStorableObject::class);

    $box = new ValidStorableObject();
    $box->example_uint64->set(456);
    $collection->add($box);
    $box = $collection->at(0);
    $this->assertEquals(456, $box->example_uint64->get());
  }

  public function testAtThrowsExceptionWhenOutOfBounds(): void {
    $collection = new VectorBase(ValidStorableObject::class);
    $this->expectException(OutOfBoundsException::class);
    $foo = $collection->at(12);
  }

  public function testSetAllOverridesAllItems(): void {
    $collection = new VectorBase(ValidStorableObject::class);

    $box = new ValidStorableObject();
    $box->example_uint64->set(12);
    $collection->add($box);
    $collection->add($box);

    $array = array(
      0 => new ValidStorableObject(),
      1 => new ValidStorableObject(),
    );
    $collection->setAll($array);

    $this->assertEquals(2, $collection->count());

    for ($cur = 0; $cur < count($array); $cur++) {
      $this->assertEquals($array[$cur], $collection->at($cur));
    }
  }

  public function testItemIsRemovedWithKey(): void {
    $collection = new VectorBase(ValidStorableObject::class);
    $box1 = new ValidStorableObject();
    $collection->add($box1);
    $box2 = new ValidStorableObject();
    $collection->add($box2);

    $collection->removeKey(0);
    $this->assertEquals($box2, $collection->at(0));
  }

  public function testContainsKeyReturnsFalseWhenItemDoesNotExist(): void {
    $collection = new VectorBase(ValidStorableObject::class);
    $this->assertFalse($collection->containsKey(0));
  }

  public function testContainsKeyReturnsTrueWhenItemExists(): void {
    $collection = new VectorBase(ValidStorableObject::class);
    $collection->add(new ValidStorableObject());
    $this->assertTrue($collection->containsKey(0));
  }

  public function testFieldsReturnValidObject(): void {
    $collection = new VectorBase(ValidStorableObject::class);
    $this->assertInstanceOf(FieldsInterface::class, $collection->fields());
  }

  //public function hideLogs(): bool { return false; }

  public function testImportReturnsValueObject(): void {

    $collection = new VectorBase(ValidStorableObject::class);

    // Should be returning the storable importer when ased to do a importer
    $importer = $collection->import();

    $this->assertInstanceOf(StorableImporter::class, $importer);

    $testObj = new ValidStorableObject();
    $testObj->example_string->set('test value');

    $importer->fromMap(
      Map {
        "mapOfArray" => Map {
          "example_string" => 'testing-string',
          "example_float" => 1.3,
        },
      },
    );
    $this->assertEquals(1, $collection->count());

    $importer->fromMap(
      Map {"mapOfArray" => Vector {'testing-string', 2, 1.3}},
    );
    $this->assertEquals(1, $collection->count());

    $importer->fromMap(
      Map {
        "mapOfArray" => array(
          "example_string" => 'testing-string',
          "example_float" => 1.3,
        ),
      },
    );
    $this->assertEquals(1, $collection->count());

    $this->expectException(UnsupportedTypeException::class);
    $importer->fromMap(Map {"mapOfArray" => $testObj});

  }

  public function testStorableImporterProtobufOff(): void {
    // Should be returning the storable importer when ased to do a importer
    $collection = new VectorBase(ValidStorableObject::class);
    $importer = $collection->import();

    $this->assertInstanceOf(StorableImporter::class, $importer);

    // assure that protobuf is off atm
    $this->expectException(OperationNotSupportedException::class);
    $importer->fromBinary('...');

  }

  public function testExportReturnsValueObject(): void {
    $collection = new VectorBase(ValidStorableObject::class);
    $this->assertInstanceOf(ExportInterface::class, $collection->export());
  }

  public function testRequiredCollectionReturnsTrue(): void {
    $collection = new VectorBase(ValidStorableObject::class);

    $collection->setIsRequired(true);
    $this->assertTrue($collection->getIsRequired());
  }

  public function testRequiredIsFalseByDefault(): void {
    $collection = new VectorBase(ValidStorableObject::class);

    $this->assertFalse($collection->getIsRequired());
  }

  public function testIsDefaultIsTrueWhenNoModificationsMade(): void {
    $collection = $this->getUnmodifiedCollection();

    list($isDefault, $fields) = $collection->isDefaultValue();
    $this->assertTrue($isDefault);
  }

  public function testUnmodifiedFieldsAreReturned(): void {
    $collection = $this->getUnmodifiedCollection();
    list($isDefault, $fields) = $collection->isDefaultValue();

    $this->assertEquals('[0]', $fields[0]);
  }

  private function getUnmodifiedCollection(): Base<ValidStorableObject> {
    $collection = new VectorBase(ValidStorableObject::class);
    $box = new ValidStorableObject();
    $collection->add($box);
    return $collection;
  }

  public function testIsDefaultIsFalseWhenModificationsMade(): void {
    $collection = new VectorBase(ValidStorableObject::class);
    $box = new ValidStorableObject();
    $box->example_uint64->set(1234);
    $collection->add($box);

    list($isDefault, $fields) = $collection->isDefaultValue();

    $this->assertFalse($isDefault);
  }

  public function testIsDefaultReturnsExpectedValueAfterSet(): void {
    $collection = new VectorBase(ValidStorableObject::class);
    $collection->setIsDefaultValue(false);

    list($isDefault, $fields) = $collection->isDefaultValue();
    $this->assertFalse($isDefault);
  }
}
