<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Map\V2\Mock;

use Zynga\Framework\StorableObject\Collections\Map\V2\Base as StorableMap;
use Zynga\Framework\StorableObject\Collections\Map\V2\Mock\ConstKeys;
use
  Zynga\Framework\StorableObject\Collections\V2\Interfaces\StorableMapCollection
;
use Zynga\Framework\Type\V1\FloatBox;

class FloatBoxMap {
  /**
   * To test functionality on Map of FloatBox type
   */
  public StorableMapCollection<FloatBox> $floatMap;

  public function __construct() {
    $this->floatMap = new StorableMap(FloatBox::class);
  }

  public static function getEmptyCollection(
  ): StorableMapCollection<FloatBox> {
    $obj = new FloatBoxMap();
    return $obj->floatMap;
  }

  public static function getRawVector(): Vector<mixed> {
    return Vector {
      ConstKeys::FLOAT_VAL_0,
      ConstKeys::FLOAT_VAL_1,
      ConstKeys::FLOAT_VAL_2,
    };
  }

  public static function getPopulatedCollectionFromVector(
  ): StorableMapCollection<FloatBox> {
    $expected = new StorableMap(FloatBox::class);

    $valBox0 = new FloatBox();
    $valBox1 = new FloatBox();
    $valBox2 = new FloatBox();

    $valBox0->set(ConstKeys::FLOAT_VAL_0);
    $valBox1->set(ConstKeys::FLOAT_VAL_1);
    $valBox2->set(ConstKeys::FLOAT_VAL_2);

    return
      $expected->set(ConstKeys::INDEXED_KEY_0, $valBox0)
        ->set(ConstKeys::INDEXED_KEY_1, $valBox1)
        ->set(ConstKeys::INDEXED_KEY_2, $valBox2);
  }
}
