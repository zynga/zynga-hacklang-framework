<?hh //strict

namespace Zynga\Framework\StorableObject\Collections\Importers\V1;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use
  Zynga\Framework\StorableObject\Collections\V1\Interfaces\StorableCollection
;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;

use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

abstract class BoxTest extends TestCase {
  public function __construct(string $name) {
    parent::__construct($name);
  }

  public function testImportFromVectorMatchesExpectedValue(): void {
    list($collection, $item, $array) = $this->getCollectionAndPayloadToTest();
    $importer = $collection->import();

    $vector = new Vector($array);
    $callbackToPayloadList = array(
      array(inst_meth($importer, 'fromVector'), $vector),
    );

    foreach ($callbackToPayloadList as $payload) {
      $payload[0]($payload[1]);
      $this->assertEquals($item, $collection->at(0));
    }
  }

  public function testImportFromEmptyVectorHasEmptyCollection(): void {
    $collection = $this->getCollection(StringBox::class);
    $importer = $collection->import();

    $importer->fromVector(Vector {});
    $this->assertEquals(0, $collection->count());
  }

  public function testImportFromNestedVectorThrowsException(): void {
    $collection = $this->getCollection(StringBox::class);
    $importer = $collection->import();

    $this->expectException(UnsupportedTypeException::class);
    $importer->fromVector(Vector {Vector {'foo'}});
  }

  public function testImportFromMapMatchesExpectedValue(): void {
    list($collection, $item, $array) = $this->getCollectionAndPayloadToTest();
    $importer = $collection->import();

    $map = Map {'foo' => $item->get()};

    $callbackToPayloadList = array(
      array(inst_meth($importer, 'fromMap'), $map),
    );

    foreach ($callbackToPayloadList as $payload) {
      $payload[0]($payload[1]);
      $this->assertEquals($item, $collection->at(0));
    }
  }

  public function testImportFromEmptyMapHasEmptyCollection(): void {
    $collection = $this->getCollection(StringBox::class);
    $importer = $collection->import();

    $importer->fromMap(Map {});
    $this->assertEquals(0, $collection->count());
  }

  public function testImportFromNestedMapThrowsException(): void {
    $collection = $this->getCollection(StringBox::class);
    $importer = $collection->import();

    $this->expectException(UnsupportedTypeException::class);
    $importer->fromMap(Map {"Map" => Map {'foo' => 'bar'}});
  }

  public function testImportFromJsonSucceeds(): void {
    list($collection, $item, $array) = $this->getCollectionAndPayloadToTest();
    $importer = $collection->import();

    $callbackToPayloadList = array(
      array(inst_meth($importer, 'fromJSON'), json_encode($array)),
      array(
        inst_meth($importer, 'fromJSON'),
        json_encode(array_combine($array, $array)),
      ),
    );

    foreach ($callbackToPayloadList as $payload) {
      $payload[0]($payload[1]);
      $this->assertEquals($item, $collection->at(0));
    }
  }

  public function testImportFromJsonFailsWhenGivenMalformedJson(): void {
    $collection = $this->getCollection(StringBox::class);
    $importer = $collection->import();

    $this->expectException(UnsupportedTypeException::class);
    $importer->fromJSON('asd}');
  }

  public function testImportFromNestedJsonThrowsException(): void {
    $collection = $this->getCollection(StringBox::class);
    $importer = $collection->import();

    $this->expectException(UnsupportedTypeException::class);
    $importer->fromJSON('{"foo":["bar","bar"]}');
  }

  public function testImportFromBinaryThrowsNotSupportedException(): void {
    $collection = $this->getCollection(StringBox::class);
    $importer = $collection->import();

    $this->expectException(OperationNotSupportedException::class);
    $importer->fromBinary('asdas');
  }

  private function getCollectionAndPayloadToTest(
  ): (StorableCollection<StringBox>, StringBox, array<mixed>) {
    $collection = $this->getCollection(StringBox::class);

    $rawString = 'foo';

    $item = new StringBox();
    $item->set($rawString);

    $json = json_encode(array($rawString));
    $payload = json_decode($json, true);
    return tuple($collection, $item, $payload);
  }

  abstract protected function getCollection<Tv as TypeInterface>(
    classname<Tv> $classname,
  ): StorableCollection<Tv>;
}
