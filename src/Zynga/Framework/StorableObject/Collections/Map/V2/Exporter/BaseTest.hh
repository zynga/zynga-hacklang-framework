<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Map\V2\Exporter;

use Zynga\Framework\Exception\V1\Exception;
use
  Zynga\Framework\StorableObject\Collections\Map\V2\Mock\Nested\MapDepth1 as DeeplyNestedMap
;
use Zynga\Framework\StorableObject\Collections\Map\V2\Mock\StorableObjectMap;
use Zynga\Framework\StorableObject\Collections\Map\V2\Mock\StringBoxMap;
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;
use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\StringBox;

class BaseTest extends TestCase {
  public function testExportAsJsonSucceedsForStringVector(): void {
    $collection = StringBoxMap::getPopulatedCollectionFromVector();

    $this->assertEquals(
      StringBoxMap::getKeyedJsonForVector(),
      $collection->export()->asJSON(),
    );
  }

  public function testExportAsJsonSucceedsForStringMap(): void {
    $collection = StringBoxMap::getPopulatedCollectionFromMap();

    $this->assertEquals(
      StringBoxMap::getJsonForMap(),
      $collection->export()->asJSON(),
    );
  }

  public function testExportAsJsonSucceedsForNestedVector(): void {
    $collection = StorableObjectMap::getPopulatedCollectionForNestedVector();

    $this->assertEquals(
      StorableObjectMap::getKeyedJsonForNestedVector(),
      $collection->export()->asJSON(),
    );
  }

  public function testExportAsJsonSucceedsForNestedMap(): void {
    $collection = StorableObjectMap::getPopulatedCollectionForNestedMap();

    $this->assertEquals(
      StorableObjectMap::getKeyedJsonForNestedMap(),
      $collection->export()->asJSON(),
    );
  }

  public function testExportAsJsonSucceedsForDeeplyNestedMap(): void {
    $collection = DeeplyNestedMap::getPopulatedCollectionForNestedMap();

    $this->assertEquals(
      DeeplyNestedMap::getJsonForNestedMap(),
      $collection->export()->asJSON(),
    );
  }

  public function testExportAsJsonAddsParentKey(): void {
    $collection = StorableObjectMap::getPopulatedCollectionForNestedMap();
    $this->assertEquals(
      StorableObjectMap::getKeyedJsonForNestedMap("parent"),
      $collection->export()->asJSON("parent"),
    );
  }

  public function testExportAsJsonSortsKeys(): void {
    $collection = StringBoxMap::getPopulatedCollectionFromMap();

    $this->assertEquals(
      StringBoxMap::getJsonForSortedMap(),
      $collection->export()->asJSON(null, true),
    );
  }

  public function testExportAsJsonSucceedsForEmptyMap(): void {
    $collection = StorableObjectMap::getEmptyCollection();

    $this->assertEquals('{}', $collection->export()->asJSON());
  }

  public function testExportAsJsonThrowsExceptionForBrokenExporter(): void {
    $collection = StorableObjectMap::getCollectionWithBrokenExporter();

    $this->expectException(Exception::class);
    $collection->export()->asJSON();
  }

  public function testExportAsBinaryThrowsNotSupportedException(): void {
    $collection = StorableObjectMap::getPopulatedCollectionForNestedMap();

    $this->expectException(OperationNotSupportedException::class);
    $collection->export()->asBinary();
  }

  public function testExportAsMapSucceedsForStringMap(): void {
    $collection = StringBoxMap::getPopulatedCollectionFromMap();

    $this->assertEquals(
      StringBoxMap::getRawMap(),
      $collection->export()->asMap(),
    );
  }

  public function testExportAsMapSucceedsForEmptyMap(): void {
    $collection = StorableObjectMap::getEmptyCollection();

    $this->assertEquals(Map {}, $collection->export()->asMap());
  }

  public function testExportAsMapThrowsExceptionForBrokenExporter(): void {
    $collection = StorableObjectMap::getCollectionWithBrokenExporter();

    $this->expectException(Exception::class);
    $collection->export()->asMap();
  }

  public function testExportAsArraySucceedsForStringArray(): void {
    $collection = StringBoxMap::getPopulatedCollectionFromMap();

    $this->assertEquals(
      StringBoxMap::getRawMap()->toArray(),
      $collection->export()->asArray(),
    );
  }

  public function testExportAsArraySucceedsForEmptyArray(): void {
    $collection = StorableObjectMap::getEmptyCollection();

    $this->assertEquals(array(), $collection->export()->asArray());
  }

  public function testExportAsArrayThrowsExceptionForBrokenExporter(): void {
    $collection = StorableObjectMap::getCollectionWithBrokenExporter();

    $this->expectException(Exception::class);
    $collection->export()->asArray();
  }
}
