<?hh //strict

namespace Zynga\Framework\StorableObject\Collections\Vector\V1;

use Zynga\Framework\Exception\V1\Exception;
use
  Zynga\Framework\StorableObject\Collections\V1\Interfaces\StorableCollection
;
use
  Zynga\Framework\StorableObject\Collections\Vector\V1\Mock\ValidVectorMock as ValidVectorMock
;
use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Nofields as NofieldsStorableObject
;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidStorableObject;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Broken\ValidButBrokenExporter
;
use Zynga\Framework\StorableObject\V1\Test\Mock\ValidNestedVector;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\UInt64Box;

class ExporterTest extends TestCase {

  protected function getCollection<Tv as StorableObjectInterface>(
    classname<Tv> $classname,
  ): StorableCollection<Tv> {
    return new Base($classname);
  }

  protected function getCollectionClassName<Tv as StorableObjectInterface>(
  ): classname<StorableCollection<Tv>> {
    return Base::class;
  }

  public function testValidJSONFromTypeInterface(): void {
    $collectionVector = new ValidVectorMock();
    $stringBox1 = new StringBox();
    $stringBox1->set('123');
    $collectionVector->stringVector->add($stringBox1);

    $stringBox2 = new StringBox();
    $stringBox2->set('234');
    $collectionVector->stringVector->add($stringBox2);

    $targetJson = '{"stringVector":["123","234"]}';
    $this->assertEquals($targetJson, $collectionVector->export()->asJSON());
  }

  public function testValidMapFromTypeInterface(): void {

    $collectionVector = new ValidVectorMock();
    $stringBox1 = new StringBox();
    $stringBox1->set('some-first-value');
    $collectionVector->stringVector->add($stringBox1);

    $stringBox2 = new StringBox();
    $stringBox2->set('some-second-value');
    $collectionVector->stringVector->add($stringBox2);

    $exportedMap = $collectionVector->stringVector->export()->asMap();
    $this->assertEquals('some-first-value', $exportedMap->get('0'));
    $this->assertEquals('some-second-value', $exportedMap->get('1'));
  }

  public function testAsJSONValid(): void {

    $vec = $this->getCollection(ValidStorableObject::class);

    $testValue = new ValidStorableObject();
    $testValue->example_uint64->set(12);
    $testValue->example_string->setIsRequired(false);
    $testValue->example_float->setIsRequired(false);

    $vec->add($testValue);

    $targetJson = '['.$testValue->export()->asJSON().']';
    $this->assertEquals($targetJson, $vec->export()->asJSON());
  }

  public function testAsJSONEmptyChild(): void {
    $vec = $this->getCollection($this->getCollectionClassName());
    $vec->setIsDefaultValue(false);

    $c_vec = $this->getCollection(ValidStorableObject::class);
    $c_vec->setIsDefaultValue(false);

    $vec->add($c_vec);

    $json = $vec->export()->asJSON();
    $this->assertEquals('[[]]', $json);
  }

  public function testAsJSONNoFields(): void {
    $vec = $this->getCollection(ValidStorableObject::class);
    $json = $vec->export()->asJSON();
    $this->assertEquals('[]', $json);
  }

  public function testCollectionCount(): void {
    $vec = $this->getCollection(ValidStorableObject::class);
    $this->assertEquals(0, $vec->count());
  }

  public function testAsJSONRecursiveStorable(): void {
    $vec = $this->getCollection(ValidStorableObject::class);
    $obj = new ValidStorableObject();
    $vec->add($obj);

    // as this is a empty object we should not see it in the realized javascript.
    $targetJson = '[]';
    $this->assertEquals($targetJson, $vec->export()->asJSON());

    $someString = 'how now brown cow?';
    $someUint64 = 238472329834;
    $someFloat = 1234.45;

    $anotherObj = new ValidStorableObject();
    $anotherObj->example_string->set($someString);
    $anotherObj->example_uint64->set($someUint64);
    $anotherObj->example_float->set($someFloat);

    $vec->add($anotherObj);

    // now we should see only one entry for the object that contains  data, the
    // empty one gets filtered out.
    $targetJson =
      '[{"example_string":"'.
      $someString.
      '","example_uint64":'.
      $someUint64.
      ',"example_float":'.
      $someFloat.
      '}]';
    $this->assertEquals($targetJson, $vec->export()->asJSON());
  }

  public function testAsMapValid(): void {

    $vec = $this->getCollection(ValidStorableObject::class);

    $testValue = new ValidStorableObject();
    $testValue->example_uint64->set(12);
    $testValue->example_string->setIsRequired(false);
    $testValue->example_float->setIsRequired(false);

    $vec->add($testValue);

    $targetMap = Map {'0' => $testValue->export()->asMap()};
    $this->assertEquals($targetMap, $vec->export()->asMap());
  }

  public function testAsMapEmptyChild(): void {
    $vec = $this->getCollection($this->getCollectionClassName());
    $vec->setIsDefaultValue(false);

    $c_vec = $this->getCollection(ValidStorableObject::class);
    $c_vec->setIsDefaultValue(false);

    $vec->add($c_vec);

    $map = $vec->export()->asMap();
    $this->assertEquals(Map {'0' => Map {}}, $map);
  }

  public function testAsMapNoFields(): void {
    $vec = $this->getCollection(ValidStorableObject::class);
    $map = $vec->export()->asMap();
    $this->assertEquals(Map {}, $map);
  }

  public function testAsMapRecursiveStorable(): void {
    $vec = $this->getCollection(ValidStorableObject::class);
    $obj = new ValidStorableObject();
    $vec->add($obj);

    // as this is a empty object we should not see it in the realized javascript.
    $targetMap = Map {};
    $this->assertEquals($targetMap, $vec->export()->asMap());

    $someString = 'how now brown cow?';
    $someUint64 = 238472329834;
    $someFloat = 1234.45;

    $anotherObj = new ValidStorableObject();
    $anotherObj->example_string->set($someString);
    $anotherObj->example_uint64->set($someUint64);
    $anotherObj->example_float->set($someFloat);

    $vec->add($anotherObj);

    // now we should see only one entry for the object that contains  data, the
    // empty one gets filtered out.
    $targetMap = Map {
      "0" => Map {
        "example_string" => $someString,
        "example_uint64" => $someUint64,
        "example_float" => $someFloat,
      },
    };
    $this->assertEquals($targetMap, $vec->export()->asMap());
    $this->assertEquals(
      json_encode($targetMap),
      json_encode($vec->export()->asMap()),
    );
  }

  public function testAsJSONNoFieldsOnChild(): void {
    $vec = $this->getCollection(NofieldsStorableObject::class);
    $obj = new NofieldsStorableObject();
    $vec->add($obj);
    $json = $vec->export()->asJSON();
    $this->assertEquals('[]', $json);
  }

  public function testAsMapNoFieldsOnChild(): void {
    $vec = $this->getCollection(NofieldsStorableObject::class);
    $obj = new NofieldsStorableObject();
    $vec->add($obj);
    $map = $vec->export()->asMap();
    $this->assertEquals(Map {}, $map);
  }

  public function testAsBinaryNotImplemented(): void {
    $vec = $this->getCollection(ValidStorableObject::class);

    $this->expectException(OperationNotSupportedException::class);
    $vec->export()->asBinary();
  }

  public function testAsJSONBrokenExporter(): void {
    $obj = new ValidButBrokenExporter();
    $obj->example_float->set(3.145);

    $map = $this->getCollection(ValidButBrokenExporter::class);
    $map->add($obj);

    $this->expectException(Exception::class);
    $map->export()->asJSON();
  }

  public function testAsMapBrokenExporter(): void {
    $obj = new ValidButBrokenExporter();
    $obj->example_float->set(3.145);

    $map = $this->getCollection(ValidButBrokenExporter::class);
    $map->add($obj);

    $this->expectException(Exception::class);
    $map->export()->asMap();
  }

}
