<?hh //strict

namespace Zynga\Framework\StorableObject\V1\Object;

use Zynga\Framework\StorableObject\V1\Test\Mock\Valid;
use Zynga\Framework\StorableObject\V1\Test\Mock\Nofields;
use Zynga\Framework\StorableObject\V1\Test\Mock\Broken\Exporter as BrokenExporter;
use Zynga\Framework\StorableObject\V1\Test\Mock\InvalidNested;
use Zynga\Framework\StorableObject\V1\Test\Mock\ValidNested;
use Zynga\Framework\StorableObject\V1\Test\Mock\InvalidUnsupportedType;
use Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired;
use Zynga\Framework\StorableObject\V1\Test\Mock\ProtobufValid;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Exception\V1\Exception;

class ExporterTest extends TestCase {

  public function test_asJSON_valid(): void {

    $obj = new Valid();

    $testStringValue = 'another_string_value_80';
    $testIntValue = 897234;
    $testFloatValue = 78575.334;

    $testVec = Vector {};
    $testVec[] = $testStringValue;
    $testVec[] = $testIntValue;
    $testVec[] = $testFloatValue;

    $obj->import()->fromVector($testVec);

    $jsonData = $obj->export()->asJSON();
    $json =
      '{"example_string":"another_string_value_80","example_uint64":897234,"example_float":78575.334}';
    $this->assertEquals($json, $jsonData);

  }

  public function test_asJSON_emptyObj_NoRequired(): void {
    $emptyObj = new ValidNoRequired();
    $this->assertEquals('{}', $emptyObj->export()->asJSON());
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function test_asJSON_invalidNested(): void {
    $obj = new InvalidNested();
    $obj->brokenExporterObj->example_float->setIsDefaultValue(false);
    $obj->export()->asJSON();
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function test_asBinary_invalidNested(): void {
    $obj = new InvalidNested();
    $obj->brokenExporterObj->example_float->setIsDefaultValue(false);
    $obj->export()->asBinary();
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function test_asJSON_invalidNestedWithRequiredChildField(): void {
    $obj = new Valid();
    $obj->example_float->setIsRequired(true);
    $obj->export()->asJSON();
  }

  public function test_asMap_valid(): void {

    $obj = new Valid();

    $testStringValue = 'another_string_value_80';
    $testIntValue = 897234;
    $testFloatValue = 78575.334;

    $testVec = Vector {};
    $testVec[] = $testStringValue;
    $testVec[] = $testIntValue;
    $testVec[] = $testFloatValue;

    $obj->import()->fromVector($testVec);

    $mapData = $obj->export()->asMap();

    $this->assertEquals($testStringValue, $mapData['example_string']);
    $this->assertEquals($testIntValue, $mapData['example_uint64']);
    $this->assertEquals($testFloatValue, $mapData['example_float']);
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function test_asMap_catchesError(): void {
    $obj = new Valid();
    $exp = new BrokenExporter($obj);
    $exp->asMap();
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function test_asBinary_catchesError(): void {
    $obj = new Valid();
    $exp = new BrokenExporter($obj);
    $exp->asBinary();
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function test_asMap_invalidNested(): void {
    $obj = new InvalidNested();
    $obj->brokenExporterObj->example_float->setIsDefaultValue(false);
    $obj->export()->asMap();
  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException
   */
  public function test_asMap_invalidUnsupportedType(): void {
    $obj = new InvalidUnsupportedType();
    $this->assertEquals(
      Exception::class,
      get_class($obj->example_nonsupported),
    );
    $obj->export()->asMap();
  }

  public function test_asBinary_valid(): void {
    $obj = new ProtobufValid();
    $this->assertEquals('', $obj->export()->asBinary());
  }

}
