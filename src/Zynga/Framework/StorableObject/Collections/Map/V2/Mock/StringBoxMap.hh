<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Map\V2\Mock;

use Zynga\Framework\StorableObject\Collections\Map\V2\Base as StorableMap;
use Zynga\Framework\StorableObject\Collections\Map\V2\Mock\ConstKeys;
use
  Zynga\Framework\StorableObject\Collections\V2\Interfaces\StorableMapCollection
;
use Zynga\Framework\Type\V1\StringBox;

class StringBoxMap {
  /**
   * To test functionality on Map of StringBox type
   */
  public StorableMapCollection<StringBox> $stringMap;

  public function __construct() {
    $this->stringMap = new StorableMap(StringBox::class);
  }

  public static function getEmptyCollection(
  ): StorableMapCollection<StringBox> {
    $obj = new StringBoxMap();
    return $obj->stringMap;
  }

  /**
   * Helper functions for vector of strings
   */ 
  public static function getRawVector(): Vector<mixed> {
    return Vector {ConstKeys::STRING_VAL_0, ConstKeys::STRING_VAL_1};
  }

  public static function getPopulatedCollectionFromVector(
  ): StorableMapCollection<StringBox> {
    $expected = new StorableMap(StringBox::class);

    $valBox0 = new StringBox();
    $valBox1 = new StringBox();

    $valBox0->set(ConstKeys::STRING_VAL_0);
    $valBox1->set(ConstKeys::STRING_VAL_1);

    return
      $expected->set(ConstKeys::INDEXED_KEY_0, $valBox0)
        ->set(ConstKeys::INDEXED_KEY_1, $valBox1);
  }

  public static function getJsonForVector(): string {
    $arr = array(ConstKeys::STRING_VAL_0, ConstKeys::STRING_VAL_1);
    return json_encode($arr);
  }

  public static function getKeyedJsonForVector(): string {
    $arr = array(
      ConstKeys::INDEXED_KEY_0 => ConstKeys::STRING_VAL_0,
      ConstKeys::INDEXED_KEY_1 => ConstKeys::STRING_VAL_1,
    );

    return json_encode($arr, JSON_FORCE_OBJECT);
  }

  /**
   * Helper functions for Map of strings
   */
  public static function getRawMap(): Map<string, mixed> {
    return Map {
      ConstKeys::STRING_KEY_1 => ConstKeys::STRING_VAL_1,
      ConstKeys::STRING_KEY_0 => ConstKeys::STRING_VAL_0,
    };
  }

  public static function getPopulatedCollectionFromMap(
  ): StorableMapCollection<StringBox> {
    $expected = new StorableMap(StringBox::class);

    $valBox0 = new StringBox();
    $valBox1 = new StringBox();

    $valBox0->set(ConstKeys::STRING_VAL_0);
    $valBox1->set(ConstKeys::STRING_VAL_1);

    return
      $expected->set(ConstKeys::STRING_KEY_1, $valBox1)
        ->set(ConstKeys::STRING_KEY_0, $valBox0);
  }

  public static function getJsonForMap(): string {
    $arr = array(
      ConstKeys::STRING_KEY_1 => ConstKeys::STRING_VAL_1,
      ConstKeys::STRING_KEY_0 => ConstKeys::STRING_VAL_0,
    );

    return json_encode($arr);
  }

  public static function getJsonForSortedMap(): string {
    $arr = array(
      ConstKeys::STRING_KEY_0 => ConstKeys::STRING_VAL_0,
      ConstKeys::STRING_KEY_1 => ConstKeys::STRING_VAL_1,
    );

    return json_encode($arr);
  }
}
