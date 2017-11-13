<?hh //strict

namespace Zynga\Framework\StorableObject\Collections\Importers\V1;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use
  Zynga\Framework\StorableObject\Collections\V1\Interfaces\StorableCollection
;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;

use Zynga\Framework\StorableObject\Co;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidStorableObject;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;

abstract class BaseTest extends TestCase {
  public function __construct(string $name) {
    parent::__construct($name);
  }

  public function testImportFromVectorMatchesExpectedValue(): void {
    list($collection, $storable, $array) =
      $this->getCollectionAndPayloadToTest();
    $importer = $collection->import();

    $callbackToPayloadList = array(
      array(inst_meth($importer, 'fromVector'), Vector {$array}),
      array(inst_meth($importer, 'fromVector'), Vector {new Vector($array)}),
    );

    foreach ($callbackToPayloadList as $payload) {
      $payload[0]($payload[1]);
      $this->assertEquals($storable, $collection->at(0));
    }
  }

  public function testImportFromEmptyVectorHasEmptyCollection(): void {
    $collection = $this->getCollection(ValidStorableObject::class);
    $importer = $collection->import();

    $importer->fromVector(Vector {});
    $this->assertEquals(0, $collection->count());
  }

  public function testImportFromInvalidVectorThrowsException(): void {
    $collection = $this->getCollection(ValidStorableObject::class);
    $importer = $collection->import();

    $this->expectException(UnsupportedTypeException::class);
    $importer->fromVector(Vector {00});
  }

  public function testImportFromMapMatchesExpectedValue(): void {
    list($collection, $storable, $array) =
      $this->getCollectionAndPayloadToTest();
    $importer = $collection->import();

    $callbackToPayloadList = array(
      array(inst_meth($importer, 'fromMap'), Map {"foo" => $array}),
      array(inst_meth($importer, 'fromMap'), Map {"foo" => new Map($array)}),
    );

    foreach ($callbackToPayloadList as $payload) {
      $payload[0]($payload[1]);
      $this->assertEquals($storable, $collection->at(0));
    }
  }

  public function testImportFromEmptyMapHasEmptyCollection(): void {
    $collection = $this->getCollection(ValidStorableObject::class);
    $importer = $collection->import();

    $importer->fromMap(Map {});
    $this->assertEquals(0, $collection->count());
  }

  public function testImportFromJsonSucceeds(): void {
    list($collection, $storable, $array) =
      $this->getCollectionAndPayloadToTest();
    $importer = $collection->import();

    $callbackToPayloadList = array(
      array(inst_meth($importer, 'fromJSON'), json_encode(array($array))),
      array(
        inst_meth($importer, 'fromJSON'),
        json_encode(array("foo" => $array)),
      ),
    );

    foreach ($callbackToPayloadList as $payload) {
      $payload[0]($payload[1]);
      $this->assertEquals($storable, $collection->at(0));
    }
  }

  public function testImportFromJsonFailsWhenGivenMalformedJson(): void {
    $collection = $this->getCollection(ValidStorableObject::class);
    $importer = $collection->import();

    $this->expectException(UnsupportedTypeException::class);
    $importer->fromJSON('asd}');
  }

  public function testImportFromInvalidJsonThrowsException(): void {
    $collection = $this->getCollection(ValidStorableObject::class);
    $importer = $collection->import();

    $this->expectException(UnsupportedTypeException::class);
    $importer->fromJSON('}asd');
  }

  public function testImportFromInvalidMapThrowsException(): void {
    $collection = $this->getCollection(ValidStorableObject::class);
    $importer = $collection->import();

    $this->expectException(UnsupportedTypeException::class);
    $importer->fromMap(Map {"Map" => 12});
  }

  public function testImportFromBinaryThrowsNotSupportedException(): void {
    $collection = $this->getCollection(ValidStorableObject::class);
    $importer = $collection->import();

    $this->expectException(OperationNotSupportedException::class);
    $importer->fromBinary('asdas');
  }

  private function getCollectionAndPayloadToTest(
  ): (StorableCollection<ValidStorableObject>,
  ValidStorableObject,
  array<mixed>,
  ) {
    $collection = $this->getCollection(ValidStorableObject::class);

    $storable = new ValidStorableObject();
    $storable->example_string->set('foo');
    $storable->example_uint64->set(1243);
    $storable->example_float->set(1.0);
    $json = $storable->export()->asJSON();
    $payload = json_decode($json, true);
    return tuple($collection, $storable, $payload);
  }

  abstract protected function getCollection<Tv as StorableObjectInterface>(
    classname<Tv> $classname,
  ): StorableCollection<Tv>;
}
