<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Map\V2\Importer;

use
  Zynga\Framework\StorableObject\Collections\Map\V2\Mock\Nested\MapDepth1 as DeeplyNestedMap
;
use Zynga\Framework\StorableObject\Collections\Map\V2\Mock\StorableObjectMap;
use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class StorableTest extends TestCase {

  public function testImportFromVectorSucceedsForNestedVector(): void {
    $collection = StorableObjectMap::getEmptyCollection();
    $vector = StorableObjectMap::getRawNestedVector();
    $collection->import()->fromVector($vector);

    $this->assertEquals(
      StorableObjectMap::getPopulatedCollectionForNestedVector(),
      $collection,
    );
  }

  public function testImportFromVectorThrowsExceptionForInvalidData(): void {
    $collection = StorableObjectMap::getEmptyCollection();

    $this->expectException(UnsupportedTypeException::class);
    $collection->import()->fromVector(Vector {"stringInsteadOfVector"});
  }

  public function testImportFromMapSucceedsForNestedMap(): void {
    $collection = StorableObjectMap::getEmptyCollection();
    $map = StorableObjectMap::getRawNestedMap();
    $collection->import()->fromMap($map);

    $this->assertEquals(
      StorableObjectMap::getPopulatedCollectionForNestedMap(),
      $collection,
    );
  }

  public function testImportFromMapSucceedsForDeeplyNestedMap(): void {
    $collection = DeeplyNestedMap::getEmptyCollection();
    $map = DeeplyNestedMap::getRawNestedMap();
    $collection->import()->fromMap($map);

    $this->assertEquals(
      DeeplyNestedMap::getPopulatedCollectionForNestedMap(),
      $collection,
    );
  }

  public function testImportFromMapThrowsExceptionForInvalidData(): void {
    $collection = StorableObjectMap::getEmptyCollection();

    $this->expectException(UnsupportedTypeException::class);
    $collection->import()->fromMap(Map {"key" => "stringInsteadOfMap"});
  }

  public function testImportFromBinaryThrowsNotSupportedException(): void {
    $collection = StorableObjectMap::getEmptyCollection();

    $this->expectException(OperationNotSupportedException::class);
    $collection->import()->fromBinary('asdas');
  }

  public function testImportFromJSONSucceedsForNestedVector(): void {
    $collection = StorableObjectMap::getEmptyCollection();
    $json = StorableObjectMap::getJsonForNestedVector();
    $collection->import()->fromJSON($json);

    $this->assertEquals(
      StorableObjectMap::getPopulatedCollectionForNestedVector(),
      $collection,
    );
  }

  public function testImportFromJSONSucceedsForNestedMap(): void {
    $collection = StorableObjectMap::getEmptyCollection();
    $json = StorableObjectMap::getJsonForNestedMap();
    $collection->import()->fromJSON($json);

    $this->assertEquals(
      StorableObjectMap::getPopulatedCollectionForNestedMap(),
      $collection,
    );
  }

  public function testImportFromJSONThrowExceptionForInvalidJSON(): void {
    $collection = StorableObjectMap::getEmptyCollection();

    $this->expectException(UnsupportedTypeException::class);
    $collection->import()->fromJSON('InvalidJson');
  }
}
