<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Map\V2\Mock\Nested;

use
  Zynga\Framework\StorableObject\Collections\Map\V2\Mock\Nested\StorableObjectDepth2
;

use Zynga\Framework\StorableObject\Collections\Map\V2\Base as StorableMap;
use Zynga\Framework\StorableObject\Collections\Map\V2\Mock\ConstKeys;
use
  Zynga\Framework\StorableObject\Collections\V2\Interfaces\StorableMapCollection
;
use
  Zynga\Framework\StorableObject\Collections\Vector\V1\Base as StorableVector
;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Broken\ValidButBrokenExporter
;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as TestStorableObject;
use Zynga\Framework\Type\V1\BoolBox;
use Zynga\Framework\Type\V1\FloatBox;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\UInt64Box;

class MapDepth1 {
  /**
   * Collection of deeply nested StorableObject type
   * testMap (Depth 1)
   * |
   * |-Map of StorableObject (Depth 2)
   *   |
   *   |- BoolVector (Depth 3)
   *   |  |
   *   |  |- Vector of BoolBox (Depth 4)
   *   |
   *   |- StorableObject (Depth 3)
   *      |
   *      |- StringMap (Depth 4)
   *      |  |
   *      |  |- Map of StringBox (Depth 5)
   *      |
   *      |- StorableMap (Depth 4)
   *         |
   *         |- Map of StorableObject (Depth 5)
   *            |
   *            |- StorableObject (Depth 6)
   *               |
   *               |- StringBox (Depth 7)
   *               |- UInt64Box (Depth 7)
   *               |- FloatBox  (Depth 7)
   *
   */
  public StorableMapCollection<StorableObjectDepth2> $testMap;

  public function __construct() {
    $this->testMap = new StorableMap(StorableObjectDepth2::class);
  }

  /**
   * Helper functions for vector related data
   */
  public static function getEmptyCollection(
  ): StorableMapCollection<StorableObjectDepth2> {
    $obj = new MapDepth1();
    return $obj->testMap;
  }

  public static function getRawNestedMap(): Map<string, mixed> {
    return new Map(self::getRawNestedArray());
  }

  private static function getRawNestedArray(): array<string, mixed> {
    return array(
      ConstKeys::STRING_KEY_0 =>
        array(
          'boolVectorDepth3' =>
            array(true, false),
          'storableObjectDepth3' =>
            array(
              'storableMapDepth4' =>
                Map {
                  ConstKeys::STRING_KEY_0 =>
                    array(
                      'storableObjectDepth6' =>
                        array(
                          StorableObjectDepth6::EXAMPLE_STRING_KEY =>
                            ConstKeys::STRING_VAL_0,
                          StorableObjectDepth6::EXAMPLE_UINT64_KEY =>
                            ConstKeys::UINT_VAL_0,
                          StorableObjectDepth6::EXAMPLE_FLOAT_KEY =>
                            ConstKeys::FLOAT_VAL_0,
                        ),
                    ),
                  ConstKeys::STRING_KEY_1 =>
                    array(
                      'storableObjectDepth6' =>
                        array(
                          StorableObjectDepth6::EXAMPLE_STRING_KEY =>
                            ConstKeys::STRING_VAL_1,
                          StorableObjectDepth6::EXAMPLE_UINT64_KEY =>
                            ConstKeys::UINT_VAL_1,
                          StorableObjectDepth6::EXAMPLE_FLOAT_KEY =>
                            ConstKeys::FLOAT_VAL_1,
                        ),
                    ),
                },
              'stringMapDepth4' =>
                array(ConstKeys::STRING_KEY_2 => ConstKeys::STRING_VAL_0),
            ),
        ),
    );
  }

  public static function getPopulatedCollectionForNestedMap(
  ): StorableMapCollection<StorableObjectDepth2> {
    $depth5Obj0 = new StorableObjectDepth5();
    $depth5Obj0->storableObjectDepth6
      ->example_string
      ->set(ConstKeys::STRING_VAL_0);
    $depth5Obj0->storableObjectDepth6
      ->example_uint64
      ->set(ConstKeys::UINT_VAL_0);
    $depth5Obj0->storableObjectDepth6
      ->example_float
      ->set(ConstKeys::FLOAT_VAL_0);

    $depth5Obj1 = new StorableObjectDepth5();
    $depth5Obj1->storableObjectDepth6
      ->example_string
      ->set(ConstKeys::STRING_VAL_1);
    $depth5Obj1->storableObjectDepth6
      ->example_uint64
      ->set(ConstKeys::UINT_VAL_1);
    $depth5Obj1->storableObjectDepth6
      ->example_float
      ->set(ConstKeys::FLOAT_VAL_1);

    $stringBox0 = new StringBox();
    $stringBox0->set(ConstKeys::STRING_VAL_0);

    $boolBox0 = new BoolBox();
    $boolBox0->set(true);

    $boolBox1 = new BoolBox();
    $boolBox1->set(false);

    $depth2Obj = new StorableObjectDepth2();
    $depth2Obj->storableObjectDepth3
      ->storableMapDepth4
      ->set(ConstKeys::STRING_KEY_0, $depth5Obj0)
      ->set(ConstKeys::STRING_KEY_1, $depth5Obj1);

    $depth2Obj->storableObjectDepth3
      ->stringMapDepth4
      ->set(ConstKeys::STRING_KEY_2, $stringBox0);

    $depth2Obj->boolVectorDepth3->add($boolBox0)->add($boolBox1);

    $expected = new StorableMap(StorableObjectDepth2::class);

    return $expected->set(ConstKeys::STRING_KEY_0, $depth2Obj);
  }

  public static function getJsonForNestedMap(): string {
    return json_encode(self::getRawNestedArray());
  }
}
