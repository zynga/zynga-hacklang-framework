<?hh //strict

namespace Zynga\Framework\StorableObject\Collections\Exporters\V1;

use
  Zynga\Framework\StorableObject\Collections\V1\Interfaces\StorableCollection
;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Exception\V1\Exception;
use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;

use Zynga\Framework\StorableObject\V1\Test\Mock\ValidNestedVector;

use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidStorableObject;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Nofields as NofieldsStorableObject
;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Broken\ValidButBrokenExporter
;

use Zynga\Framework\Type\V1\UInt64Box;

abstract class BaseTest extends TestCase {

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
    $this->assertEquals('[]', $json);
  }

  public function testAsJSONNoFields(): void {
    $vec = $this->getCollection(ValidStorableObject::class);
    $this->expectException(NoFieldsFoundException::class);
    $json = $vec->export()->asJSON();
  }

  public function testAsJSONRecursiveStorable(): void {
    $vec = $this->getCollection(ValidStorableObject::class);
    $obj = new ValidStorableObject();
    $vec->add($obj);

    // as thhis is a empty object we should not see it in the realized javascript.
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

  public function testAsJSONNoFieldsOnChild(): void {
    $vec = $this->getCollection(NofieldsStorableObject::class);
    $obj = new NofieldsStorableObject();
    $vec->add($obj);
    $json = $vec->export()->asJSON();
    $this->assertEquals('[]', $json);
  }

  public function testAsMapNoFields(): void {
    $vec = $this->getCollection(ValidStorableObject::class);

    $this->expectException(OperationNotSupportedException::class);
    $vec->export()->asMap();
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

  abstract protected function getCollectionClassName<Tv as StorableObjectInterface>(
  ): classname<StorableCollection<Tv>>;

  abstract protected function getCollection<Tv as StorableObjectInterface>(
    classname<Tv> $classname,
  ): StorableCollection<Tv>;

}
