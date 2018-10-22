<?hh //strict

namespace Zynga\Framework\StorableObject\Collections\Map\V2;

use \OutOfBoundsException;
use Zynga\Framework\StorableObject\Collections\Map\V2\Base as MapBase;
use Zynga\Framework\StorableObject\Collections\Map\V2\Mock\UnsupportedType;
use
  Zynga\Framework\StorableObject\Collections\Map\V2\Importer\Box as BoxImporter
;
use
  Zynga\Framework\StorableObject\Collections\Map\V2\Importer\Storable as StorableImporter
;
use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;
use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidStorableObject;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\StringBox;

class BaseTest extends TestCase {
  const string KEY_1 = "key1";
  const string KEY_2 = "key2";
  const string KEY_3 = "key3";

  public function testConstructionWithBoxSucceeds(): void {
    $collection = new MapBase(ValidStorableObject::class);
    $this->addToAssertionCount(1);
  }

  public function testConstructionFailsForUnsupportedType(): void {
    $this->expectException(OperationNotSupportedException::class);
    $collection = new MapBase(UnsupportedType::class);
  }

  public function testAddInsertsItemWithNewKey(): void {
    $collection = new MapBase(ValidStorableObject::class);

    $box1 = new ValidStorableObject();
    $box1->example_uint64->set(12);
    $collection->add(Pair {self::KEY_1, $box1});

    $box2 = new ValidStorableObject();
    $box2->example_uint64->set(23);
    $collection->add(Pair {self::KEY_2, $box2});

    $this->assertEquals(2, $collection->count());
    $this->assertEquals($box1, $collection->at(self::KEY_1));
    $this->assertEquals($box2, $collection->at(self::KEY_2));
  }

  public function testAddOverwritesItemWithExistingKey(): void {
    $collection = new MapBase(ValidStorableObject::class);

    $box1 = new ValidStorableObject();
    $box1->example_uint64->set(12);
    $collection->add(Pair {self::KEY_1, $box1});

    $this->assertEquals(1, $collection->count());
    $this->assertEquals($box1, $collection->at(self::KEY_1));

    $box2 = new ValidStorableObject();
    $box2->example_uint64->set(23);
    $collection->add(Pair {self::KEY_1, $box2});

    $this->assertEquals(1, $collection->count());
    $this->assertEquals($box2, $collection->at(self::KEY_1));
  }

  public function testSetInsertsItemWithNewKey(): void {
    $collection = new MapBase(ValidStorableObject::class);

    $box1 = new ValidStorableObject();
    $box1->example_uint64->set(12);
    $collection->add(Pair {self::KEY_1, $box1});

    $box2 = new ValidStorableObject();
    $box2->example_uint64->set(23);
    $collection->add(Pair {self::KEY_2, $box2});

    $this->assertEquals(2, $collection->count());
    $this->assertEquals($box1, $collection->at(self::KEY_1));
    $this->assertEquals($box2, $collection->at(self::KEY_2));
  }

  public function testSetOverwritesItemWithExistingKey(): void {
    $collection = new MapBase(ValidStorableObject::class);

    $box1 = new ValidStorableObject();
    $box1->example_uint64->set(12);
    $collection->add(Pair {self::KEY_1, $box1});

    $this->assertEquals(1, $collection->count());
    $this->assertEquals($box1, $collection->at(self::KEY_1));

    $box2 = new ValidStorableObject();
    $box2->example_uint64->set(23);
    $collection->add(Pair {self::KEY_1, $box2});

    $this->assertEquals(1, $collection->count());
    $this->assertEquals($box2, $collection->at(self::KEY_1));
  }

  public function testIsEmptyReturnsTrueWhenCollectionIsEmpty(): void {
    $collection = new MapBase(ValidStorableObject::class);
    $this->assertTrue($collection->isEmpty());
  }

  public function testIsEmptyReturnsFalseWhenCollectionIsNotEmpty(): void {
    $collection = new MapBase(ValidStorableObject::class);
    $collection->set(self::KEY_1, new ValidStorableObject());

    $this->assertFalse($collection->isEmpty());
  }

  public function testCountReturnsExpectedValue(): void {
    $collection = new MapBase(ValidStorableObject::class);
    $collection->set(self::KEY_1, new ValidStorableObject());

    $this->assertEquals(1, $collection->count());

    $collection->set(self::KEY_2, new ValidStorableObject());

    $this->assertEquals(2, $collection->count());
  }

  public function testIteratorWithAllItemsIsReturned(): void {
    $collection = new MapBase(ValidStorableObject::class);

    $collection->set(self::KEY_1, new ValidStorableObject())
      ->set(self::KEY_2, new ValidStorableObject())
      ->set(self::KEY_3, new ValidStorableObject());

    $items = $collection->items();

    $cur = 0;
    foreach ($items as $key => $entry) {
      ++$cur;
      $this->assertEquals("key".$cur, $key);
    }

    $this->assertEquals(3, $cur);
  }

  public function testClearRemovesAllItems(): void {
    $collection = new MapBase(ValidStorableObject::class);

    $collection->set(self::KEY_1, new ValidStorableObject());
    $collection->clear();

    $this->assertEquals(0, $collection->count());
  }

  public function testAddAllInsertsItemsFromArray(): void {
    $collection = new MapBase(ValidStorableObject::class);
    $array = array(
      Pair {self::KEY_1, new ValidStorableObject()},
      Pair {self::KEY_2, new ValidStorableObject()},
    );
    $collection->addAll($array);
    $this->assertEquals(2, $collection->count());
  }

  public function testAtReturnsExpectedValue(): void {
    $collection = new MapBase(ValidStorableObject::class);

    $box = new ValidStorableObject();
    $box->example_uint64->set(456);
    $collection->set(self::KEY_1, $box);
    $box = $collection->at(self::KEY_1);
    $this->assertEquals(456, $box->example_uint64->get());
  }

  public function testAtThrowsExceptionWhenOutOfBounds(): void {
    $collection = new MapBase(ValidStorableObject::class);
    $this->expectException(OutOfBoundsException::class);
    $foo = $collection->at(self::KEY_1);
  }

  public function testSetAllOverridesAllItems(): void {
    $collection = new MapBase(ValidStorableObject::class);

    $box = new ValidStorableObject();
    $box->example_uint64->set(12);
    $collection->set(self::KEY_1, $box)->set(self::KEY_2, $box);

    $array = array(
      self::KEY_1 => new ValidStorableObject(),
      self::KEY_2 => new ValidStorableObject(),
    );
    $collection->setAll($array);

    $this->assertEquals(2, $collection->count());

    $this->assertEquals($array[self::KEY_1], $collection->at(self::KEY_1));
    $this->assertEquals($array[self::KEY_2], $collection->at(self::KEY_2));
  }

  public function testItemIsRemovedWithRemove(): void {
    $collection = new MapBase(ValidStorableObject::class);
    $collection->set(self::KEY_1, new ValidStorableObject())
      ->set(self::KEY_2, new ValidStorableObject());

    $this->assertEquals(2, $collection->count());
    $this->assertNotNull($collection->get(self::KEY_1));
    $this->assertNotNull($collection->get(self::KEY_2));

    $collection->remove(self::KEY_1);
    $this->assertEquals(1, $collection->count());
    $this->assertNull($collection->get(self::KEY_1));
    $this->assertNotNull($collection->get(self::KEY_2));
  }

  public function testItemIsRemovedWithRemoveKey(): void {
    $collection = new MapBase(ValidStorableObject::class);
    $collection->set(self::KEY_1, new ValidStorableObject())
      ->set(self::KEY_2, new ValidStorableObject());

    $this->assertEquals(2, $collection->count());
    $this->assertNotNull($collection->get(self::KEY_1));
    $this->assertNotNull($collection->get(self::KEY_2));

    $collection->removeKey(self::KEY_1);
    $this->assertEquals(1, $collection->count());
    $this->assertNull($collection->get(self::KEY_1));
    $this->assertNotNull($collection->get(self::KEY_2));
  }

  public function testContainsKeyReturnsFalseWhenItemDoesNotExist(): void {
    $collection = new MapBase(ValidStorableObject::class);
    $this->assertFalse($collection->containsKey(0));
  }

  public function testContainsKeyReturnsTrueWhenItemExists(): void {
    $collection = new MapBase(ValidStorableObject::class);
    $collection->set(self::KEY_1, new ValidStorableObject());
    $this->assertTrue($collection->containsKey(self::KEY_1));
  }

  public function testToArrayReturnsValidArray(): void {
    $collection = new MapBase(StringBox::class);
    $obj1 = new StringBox();
    $obj1->set("value1");
    $obj2 = new StringBox();
    $obj2->set("value2");

    $collection->set(self::KEY_1, $obj1)->set(self::KEY_2, $obj2);

    $arr = $collection->toArray();

    $this->assertEquals("value1", $arr[self::KEY_1]);
    $this->assertEquals("value2", $arr[self::KEY_2]);
  }

  public function testFieldsReturnValidObject(): void {
    $collection = new MapBase(ValidStorableObject::class);
    $this->assertInstanceOf(FieldsInterface::class, $collection->fields());
  }

  public function testValidImporterIsReturnedForStorableValue(): void {
    $collection = new MapBase(ValidStorableObject::class);
    $importer = $collection->import();

    $this->assertInstanceOf(StorableImporter::class, $importer);
  }

  public function testValidImporterIsReturnedForTypeValue(): void {
    $collection = new MapBase(StringBox::class);
    $importer = $collection->import();

    $this->assertInstanceOf(BoxImporter::class, $importer);
  }

  public function testExportReturnsValueObject(): void {
    $collection = new MapBase(ValidStorableObject::class);
    $this->assertInstanceOf(ExportInterface::class, $collection->export());
  }

  public function testRequiredCollectionReturnsTrue(): void {
    $collection = new MapBase(ValidStorableObject::class);

    $collection->setIsRequired(true);
    $this->assertTrue($collection->getIsRequired());
  }

  public function testRequiredIsFalseByDefault(): void {
    $collection = new MapBase(ValidStorableObject::class);

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

    $this->assertEquals(self::KEY_1, $fields[0]);
  }

  public function testIsDefaultIsFalseWhenModificationsMade(): void {
    $collection = new MapBase(ValidStorableObject::class);
    $box = new ValidStorableObject();
    $box->example_uint64->set(1234);
    $collection->set(self::KEY_1, $box);

    list($isDefault, $fields) = $collection->isDefaultValue();

    $this->assertFalse($isDefault);
  }

  public function testIsDefaultReturnsExpectedValueAfterSet(): void {
    $collection = new MapBase(ValidStorableObject::class);
    $collection->setIsDefaultValue(false);

    list($isDefault, $fields) = $collection->isDefaultValue();
    $this->assertFalse($isDefault);
  }

  private function getUnmodifiedCollection(): Base<ValidStorableObject> {
    $collection = new MapBase(ValidStorableObject::class);
    $box = new ValidStorableObject();
    $collection->add(Pair {self::KEY_1, $box});
    return $collection;
  }
}
