<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Map\V2\Importer;

use Zynga\Framework\StorableObject\Collections\Map\V2\Mock\FloatBoxMap;
use Zynga\Framework\StorableObject\Collections\Map\V2\Mock\StringBoxMap;
use Zynga\Framework\StorableObject\Collections\Map\V2\Mock\UnixTimestampBoxMap;
use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class BoxTest extends TestCase {
  public function testImportFromVectorSucceedsForString(): void {
    $collection = StringBoxMap::getEmptyCollection();
    $vector = StringBoxMap::getRawVector();
    $collection->import()->fromVector($vector);

    $this->assertEquals(
      StringBoxMap::getPopulatedCollectionFromVector(),
      $collection,
    );
  }

  public function testImportFromVectorSucceedsForUnixTimestamp(): void {
    $collection = UnixTimestampBoxMap::getEmptyCollection();
    $vector = UnixTimestampBoxMap::getRawVector();
    $collection->import()->fromVector($vector);

    $this->assertEquals(
      UnixTimestampBoxMap::getPopulatedCollectionFromVector(),
      $collection,
    );
  }

  public function testImportFromVectorSucceedsForFloat(): void {
    $collection = FloatBoxMap::getEmptyCollection();
    $vector = FloatBoxMap::getRawVector();
    $collection->import()->fromVector($vector);

    $this->assertEquals(
      FloatBoxMap::getPopulatedCollectionFromVector(),
      $collection,
    );
  }

  public function testImportFromVectorSucceedsForEmpty(): void {
    $collection = StringBoxMap::getEmptyCollection();
    $collection->import()->fromVector(Vector {});

    $this->assertEquals(StringBoxMap::getEmptyCollection(), $collection);
  }

  public function testImportFromVectorThrowsExceptionForInvalidType(): void {
    $collection = UnixTimestampBoxMap::getEmptyCollection();
    $vector = FloatBoxMap::getRawVector();

    $this->expectException(UnsupportedTypeException::class);
    $collection->import()->fromVector($vector);
  }

  public function testImportFromMapSucceedsForString(): void {
    $collection = StringBoxMap::getEmptyCollection();
    $map = StringBoxMap::getRawMap();
    $collection->import()->fromMap($map);

    $this->assertEquals(
      StringBoxMap::getPopulatedCollectionFromMap(),
      $collection,
    );
  }

  public function testImportFromMapSucceedsForEmpty(): void {
    $collection = StringBoxMap::getEmptyCollection();
    $collection->import()->fromMap(Map {});

    $this->assertEquals(StringBoxMap::getEmptyCollection(), $collection);
  }

  public function testImportFromMapThrowsExceptionForInvalidType(): void {
    $collection = UnixTimestampBoxMap::getEmptyCollection();
    $map = StringBoxMap::getRawMap();

    $this->expectException(UnsupportedTypeException::class);
    $collection->import()->fromMap($map);
  }

  public function testImportFromBinaryThrowsNotSupportedException(): void {
    $collection = StringBoxMap::getEmptyCollection();

    $this->expectException(OperationNotSupportedException::class);
    $collection->import()->fromBinary('asdas');
  }

  public function testImportFromJSONSucceedsForStringVector(): void {
    $collection = StringBoxMap::getEmptyCollection();
    $json = StringBoxMap::getJsonForVector();
    $collection->import()->fromJSON($json);

    $this->assertEquals(
      StringBoxMap::getPopulatedCollectionFromVector(),
      $collection,
    );
  }

  public function testImportFromJSONSucceedsForStringMap(): void {
    $collection = StringBoxMap::getEmptyCollection();
    $json = StringBoxMap::getJsonForMap();
    $collection->import()->fromJSON($json);

    $this->assertEquals(
      StringBoxMap::getPopulatedCollectionFromMap(),
      $collection,
    );
  }

  public function testImportFromJSONThrowExceptionForInvalidJSON(): void {
    $collection = StringBoxMap::getEmptyCollection();

    $this->expectException(UnsupportedTypeException::class);
    $collection->import()->fromJSON('InvalidJson');
  }
}
