<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Map\V2\Mock;

use Zynga\Framework\StorableObject\Collections\Map\V2\Base as StorableMap;
use Zynga\Framework\StorableObject\Collections\Map\V2\Mock\ConstKeys;
use
  Zynga\Framework\StorableObject\Collections\V2\Interfaces\StorableMapCollection
;
use Zynga\Framework\Type\V1\UnixTimestampBox;

class UnixTimestampBoxMap {
  /**
   * To test functionality on Map of UnitTimestampBox type
   */
  public StorableMapCollection<UnixTimestampBox> $unixTimestampMap;

  public function __construct() {
    $this->unixTimestampMap = new StorableMap(UnixTimestampBox::class);
  }

  public static function getEmptyCollection(
  ): StorableMapCollection<UnixTimestampBox> {
    $obj = new UnixTimestampBoxMap();
    return $obj->unixTimestampMap;
  }

  public static function getRawVector(): Vector<mixed> {
    return Vector {
      ConstKeys::UNIX_TS_VAL_0,
      ConstKeys::UNIX_TS_VAL_1,
      ConstKeys::UNIX_TS_VAL_2,
      ConstKeys::UNIX_TS_VAL_3,
    };
  }

  public static function getPopulatedCollectionFromVector(
  ): StorableMapCollection<UnixTimestampBox> {
    $expected = new StorableMap(UnixTimestampBox::class);

    $valBox0 = new UnixTimestampBox();
    $valBox1 = new UnixTimestampBox();
    $valBox2 = new UnixTimestampBox();
    $valBox3 = new UnixTimestampBox();

    $valBox0->set(ConstKeys::UNIX_TS_VAL_0);
    $valBox1->set(ConstKeys::UNIX_TS_VAL_1);
    $valBox2->set(ConstKeys::UNIX_TS_VAL_2);
    $valBox3->set(ConstKeys::UNIX_TS_VAL_3);

    return
      $expected->set(ConstKeys::INDEXED_KEY_0, $valBox0)
        ->set(ConstKeys::INDEXED_KEY_1, $valBox1)
        ->set(ConstKeys::INDEXED_KEY_2, $valBox2)
        ->set(ConstKeys::INDEXED_KEY_3, $valBox3);
  }
}
