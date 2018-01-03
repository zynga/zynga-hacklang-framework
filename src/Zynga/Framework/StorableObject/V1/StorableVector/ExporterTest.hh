<?hh //strict

namespace Zynga\Framework\StorableObject\V1\StorableVector;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\StorableObject\V1\StorableVector;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidStorableObject;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Nofields as NofieldsStorableObject
;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Broken\ValidButBrokenExporter
;

use Zynga\Framework\Type\V1\UInt64Box;

class ExporterTest extends TestCase {

  public function test_asJSON_valid(): void {

    $randomValue = mt_rand(1, 1000000);

    $vec = new StorableVector();

    $testValue = new UInt64Box();
    $testValue->set($randomValue);

    $vec->add($testValue);

    $targetJson = '['.json_encode($randomValue).']';
    $this->assertEquals($targetJson, $vec->export()->asJSON());

  }

  public function test_asJSON_emptyChild(): void {

    $vec = new StorableVector();
    $vec->setIsDefaultValue(false);

    $c_vec = new StorableVector();
    $c_vec->setIsDefaultValue(false);

    $this->assertTrue($vec->add($c_vec));

    $json = $vec->export()->asJSON();
    $this->assertEquals('[]', $json);
  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException
   */
  public function test_asJSON_noFields(): void {
    $vec = new StorableVector();
    $json = $vec->export()->asJSON();
  }

  public function test_asJSON_recursiveStorable(): void {
    $vec = new StorableVector();
    $obj = new ValidStorableObject();
    $this->assertTrue($vec->add($obj));

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

    $this->assertTrue($vec->add($anotherObj));

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

  public function test_asJSON_noFieldsOnChild(): void {
    $vec = new StorableVector();
    $obj = new NofieldsStorableObject();
    $this->assertTrue($vec->add($obj));
    $json = $vec->export()->asJSON();
    $this->assertEquals('[]', $json);
  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
   */
  public function test_asMap_noFields(): void {
    $vec = new StorableVector();
    $vec->export()->asMap();
  }

  public function test_asBinary(): void {
    // TODO: binary isn't fully done as the protobuf support doesn't currently support repeating elements.
    $randomValue = mt_rand(1, 1000000);
    $vec = new StorableVector();
    $testValue = new UInt64Box();
    $testValue->set($randomValue);

    $targetBin = '';
    $this->assertEquals($targetBin, $vec->export()->asBinary());
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function test_asJSON_brokenExporter(): void {

    $obj = new ValidButBrokenExporter();
    $obj->example_float->set(3.145);

    $map = new StorableVector();
    $map->add($obj);

    $map->export()->asJSON();

  }

}
