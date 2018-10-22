<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Map\V2\Mock;

use Zynga\Framework\StorableObject\Collections\Map\V2\Base as StorableMap;
use Zynga\Framework\StorableObject\Collections\Map\V2\Mock\ConstKeys;
use
  Zynga\Framework\StorableObject\Collections\V2\Interfaces\StorableMapCollection
;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Broken\ValidButBrokenExporter
;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as TestStorableObject;
use Zynga\Framework\Type\V1\FloatBox;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\UInt64Box;

class StorableObjectMap {
  /**
   * Collection of StorableObject type
   */
  public StorableMapCollection<TestStorableObject> $testMap;

  public function __construct() {
    $this->testMap = new StorableMap(TestStorableObject::class);
  }

  /**
   * Helper functions for vector related data
   */
  public static function getEmptyCollection(
  ): StorableMapCollection<TestStorableObject> {
    $obj = new StorableObjectMap();
    return $obj->testMap;
  }

  public static function getRawNestedVector(): Vector<mixed> {
    return Vector {
      Vector {ConstKeys::STRING_VAL_0, ConstKeys::UINT_VAL_0, ConstKeys::FLOAT_VAL_0},
      Map {
        TestStorableObject::EXAMPLE_STRING_KEY => ConstKeys::STRING_VAL_1,
        TestStorableObject::EXAMPLE_UINT64_KEY => ConstKeys::UINT_VAL_1,
        TestStorableObject::EXAMPLE_FLOAT_KEY => ConstKeys::FLOAT_VAL_1,
      },
      array(
        TestStorableObject::EXAMPLE_STRING_KEY => ConstKeys::STRING_VAL_2,
        TestStorableObject::EXAMPLE_UINT64_KEY => ConstKeys::UINT_VAL_2,
        TestStorableObject::EXAMPLE_FLOAT_KEY => ConstKeys::FLOAT_VAL_2,
      ),
    };
  }

  public static function getPopulatedCollectionForNestedVector(
  ): StorableMapCollection<TestStorableObject> {
    $expected = new StorableMap(TestStorableObject::class);

    $testObj0 = new TestStorableObject();
    $testObj1 = new TestStorableObject();
    $testObj2 = new TestStorableObject();

    $testObj0->example_string->set(ConstKeys::STRING_VAL_0);
    $testObj0->example_uint64->set(ConstKeys::UINT_VAL_0);
    $testObj0->example_float->set(ConstKeys::FLOAT_VAL_0);

    $testObj1->example_string->set(ConstKeys::STRING_VAL_1);
    $testObj1->example_uint64->set(ConstKeys::UINT_VAL_1);
    $testObj1->example_float->set(ConstKeys::FLOAT_VAL_1);

    $testObj2->example_string->set(ConstKeys::STRING_VAL_2);
    $testObj2->example_uint64->set(ConstKeys::UINT_VAL_2);
    $testObj2->example_float->set(ConstKeys::FLOAT_VAL_2);

    return
      $expected->set(ConstKeys::INDEXED_KEY_0, $testObj0)
        ->set(ConstKeys::INDEXED_KEY_1, $testObj1)
        ->set(ConstKeys::INDEXED_KEY_2, $testObj2);
  }

  public static function getJsonForNestedVector(): string {
    $arr = array(
      array(ConstKeys::STRING_VAL_0, ConstKeys::UINT_VAL_0, ConstKeys::FLOAT_VAL_0),
      array(
        TestStorableObject::EXAMPLE_STRING_KEY => ConstKeys::STRING_VAL_1,
        TestStorableObject::EXAMPLE_UINT64_KEY => ConstKeys::UINT_VAL_1,
        TestStorableObject::EXAMPLE_FLOAT_KEY => ConstKeys::FLOAT_VAL_1,
      ),
      array(
        TestStorableObject::EXAMPLE_STRING_KEY => ConstKeys::STRING_VAL_2,
        TestStorableObject::EXAMPLE_UINT64_KEY => ConstKeys::UINT_VAL_2,
        TestStorableObject::EXAMPLE_FLOAT_KEY => ConstKeys::FLOAT_VAL_2,
      ),
    );

    return json_encode($arr);
  }

  public static function getKeyedJsonForNestedVector(): string {
    $arr = array(
      ConstKeys::INDEXED_KEY_0 => array(
        TestStorableObject::EXAMPLE_STRING_KEY => ConstKeys::STRING_VAL_0,
        TestStorableObject::EXAMPLE_UINT64_KEY => ConstKeys::UINT_VAL_0,
        TestStorableObject::EXAMPLE_FLOAT_KEY => ConstKeys::FLOAT_VAL_0,
      ),
      ConstKeys::INDEXED_KEY_1 => array(
        TestStorableObject::EXAMPLE_STRING_KEY => ConstKeys::STRING_VAL_1,
        TestStorableObject::EXAMPLE_UINT64_KEY => ConstKeys::UINT_VAL_1,
        TestStorableObject::EXAMPLE_FLOAT_KEY => ConstKeys::FLOAT_VAL_1,
      ),
      ConstKeys::INDEXED_KEY_2 => array(
        TestStorableObject::EXAMPLE_STRING_KEY => ConstKeys::STRING_VAL_2,
        TestStorableObject::EXAMPLE_UINT64_KEY => ConstKeys::UINT_VAL_2,
        TestStorableObject::EXAMPLE_FLOAT_KEY => ConstKeys::FLOAT_VAL_2,
      ),
    );

    return json_encode($arr, JSON_FORCE_OBJECT);
  }

  /**
   * Helper functions for Map related data
   */
  public static function getRawNestedMap(): Map<string, mixed> {
    $vector = self::getRawNestedVector();
    return Map {
      ConstKeys::STRING_KEY_0 => $vector->at(0),
      ConstKeys::STRING_KEY_1 => $vector->at(1),
      ConstKeys::STRING_KEY_2 => $vector->at(2),
    };
  }

  public static function getPopulatedCollectionForNestedMap(
  ): StorableMapCollection<TestStorableObject> {
    $expected = new StorableMap(TestStorableObject::class);

    $testObj0 = new TestStorableObject();
    $testObj1 = new TestStorableObject();
    $testObj2 = new TestStorableObject();

    $testObj0->example_string->set(ConstKeys::STRING_VAL_0);
    $testObj0->example_uint64->set(ConstKeys::UINT_VAL_0);
    $testObj0->example_float->set(ConstKeys::FLOAT_VAL_0);

    $testObj1->example_string->set(ConstKeys::STRING_VAL_1);
    $testObj1->example_uint64->set(ConstKeys::UINT_VAL_1);
    $testObj1->example_float->set(ConstKeys::FLOAT_VAL_1);

    $testObj2->example_string->set(ConstKeys::STRING_VAL_2);
    $testObj2->example_uint64->set(ConstKeys::UINT_VAL_2);
    $testObj2->example_float->set(ConstKeys::FLOAT_VAL_2);

    return
      $expected->set(ConstKeys::STRING_KEY_0, $testObj0)
        ->set(ConstKeys::STRING_KEY_1, $testObj1)
        ->set(ConstKeys::STRING_KEY_2, $testObj2);
  }

  public static function getJsonForNestedMap(): string {
    $arr = array(
      ConstKeys::STRING_KEY_0 => array(
        ConstKeys::STRING_VAL_0,
        ConstKeys::UINT_VAL_0,
        ConstKeys::FLOAT_VAL_0,
      ),
      ConstKeys::STRING_KEY_1 => array(
        TestStorableObject::EXAMPLE_STRING_KEY => ConstKeys::STRING_VAL_1,
        TestStorableObject::EXAMPLE_UINT64_KEY => ConstKeys::UINT_VAL_1,
        TestStorableObject::EXAMPLE_FLOAT_KEY => ConstKeys::FLOAT_VAL_1,
      ),
      ConstKeys::STRING_KEY_2 => array(
        TestStorableObject::EXAMPLE_STRING_KEY => ConstKeys::STRING_VAL_2,
        TestStorableObject::EXAMPLE_UINT64_KEY => ConstKeys::UINT_VAL_2,
        TestStorableObject::EXAMPLE_FLOAT_KEY => ConstKeys::FLOAT_VAL_2,
      ),
    );

    return json_encode($arr);
  }

  public static function getKeyedJsonForNestedMap(
    string $parent = '',
  ): string {
    $dataArr = array(
      ConstKeys::STRING_KEY_0 => array(
        TestStorableObject::EXAMPLE_STRING_KEY => ConstKeys::STRING_VAL_0,
        TestStorableObject::EXAMPLE_UINT64_KEY => ConstKeys::UINT_VAL_0,
        TestStorableObject::EXAMPLE_FLOAT_KEY => ConstKeys::FLOAT_VAL_0,
      ),
      ConstKeys::STRING_KEY_1 => array(
        TestStorableObject::EXAMPLE_STRING_KEY => ConstKeys::STRING_VAL_1,
        TestStorableObject::EXAMPLE_UINT64_KEY => ConstKeys::UINT_VAL_1,
        TestStorableObject::EXAMPLE_FLOAT_KEY => ConstKeys::FLOAT_VAL_1,
      ),
      ConstKeys::STRING_KEY_2 => array(
        TestStorableObject::EXAMPLE_STRING_KEY => ConstKeys::STRING_VAL_2,
        TestStorableObject::EXAMPLE_UINT64_KEY => ConstKeys::UINT_VAL_2,
        TestStorableObject::EXAMPLE_FLOAT_KEY => ConstKeys::FLOAT_VAL_2,
      ),
    );

    $arr = $dataArr;
    if ($parent != '') {
      $arr = array($parent => $dataArr);
    }

    return json_encode($arr, JSON_FORCE_OBJECT);
  }

  /**
   * Helper function for collection with broken exporter
   */
  public static function getCollectionWithBrokenExporter(
  ): StorableMapCollection<ValidButBrokenExporter> {
    $obj = new ValidButBrokenExporter();
    $obj->example_string->set(ConstKeys::STRING_VAL_0);
    $obj->example_uint64->set(ConstKeys::UINT_VAL_0);
    $obj->example_float->set(ConstKeys::FLOAT_VAL_0);

    $collection = new StorableMap(ValidButBrokenExporter::class);
    $collection->set(ConstKeys::STRING_KEY_0, $obj);

    return $collection;
  }
}
