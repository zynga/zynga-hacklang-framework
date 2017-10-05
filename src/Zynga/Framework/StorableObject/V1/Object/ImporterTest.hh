<?hh //strict

namespace Zynga\Framework\StorableObject\V1\Object;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\StorableObject\V1\Test\Mock\Valid;
use Zynga\Framework\StorableObject\V1\Test\Mock\InvalidUnsupportedType;
use Zynga\Framework\StorableObject\V1\Test\Mock\ValidNested;

use Zynga\Framework\Exception\V1\Exception;

class ImporterTest extends TestCase {

  public function testImportFromVector(): void {

    $obj = new Valid();

    $testStringValue = 'another_string_value_80';
    $testIntValue = 897234;
    $testFloatValue = 78575.334;

    $testVec = Vector {};
    $testVec[] = $testStringValue;
    $testVec[] = $testIntValue;
    $testVec[] = $testFloatValue;

    $obj->import()->fromVector($testVec);

    $this->assertEquals($testStringValue, $obj->example_string->get());
    $this->assertEquals($testIntValue, $obj->example_uint64->get());
    $this->assertEquals($testFloatValue, $obj->example_float->get());

  }

  public function testFromBinary(): void {
    $obj = new Valid();
    $this->assertFalse($obj->import()->fromBinary(''));
  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException
   */
  public function testVectorOfObjects(): void {

    $data = Vector {};
    $vecObj = new Valid();
    $data[] = $vecObj;
    $data[] = $vecObj;
    $data[] = $vecObj;

    $obj = new Valid();
    $obj->import()->fromVector($data);

  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException
   */
  public function testMapOfObjects(): void {

    $data = Map {};
    $vecObj = new Valid();
    $data['example_string'] = $vecObj;
    $data['example_float'] = $vecObj;
    $data['example_uint64'] = $vecObj;

    $obj = new Valid();
    $obj->import()->fromMap($data);

  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\ExpectedFieldCountMismatchException
   */
  public function testInvalidVectorImport(): void {
    $data = Vector {};
    $obj = new Valid();
    $obj->import()->fromVector($data);
  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException
   */
  public function testInvalidUnsupportedType_VectorImport(): void {
    $data = Vector {};
    $data[] = null;
    $obj = new InvalidUnsupportedType();
    $this->assertEquals(
      Exception::class,
      get_class($obj->example_nonsupported),
    );
    $obj->import()->fromVector($data);
  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\MissingKeyFromImportDataException
   */
  public function testInvalidMapImport(): void {
    $data = Map {};
    $obj = new Valid();
    $obj->import()->fromMap($data);
  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException
   */
  public function testInvalidUnsupportedType_MapImport(): void {
    $data = Map {};
    $data['example_nonsupported'] = null;
    $obj = new InvalidUnsupportedType();
    $obj->import()->fromMap($data);
  }

  public function test_JSON_import(): void {
    $json =
      '{"example_string":"another_string_value_80","example_uint64":897234,"example_float":78575.334}';
    $obj = new Valid();
    $obj->import()->fromJSON($json);

    $testStringValue = 'another_string_value_80';
    $testIntValue = 897234;
    $testFloatValue = 78575.334;

    $this->assertEquals($testStringValue, $obj->example_string->get());
    $this->assertEquals($testIntValue, $obj->example_uint64->get());
    $this->assertEquals($testFloatValue, $obj->example_float->get());

  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\MissingKeyFromImportDataException
   */
  public function test_JSON_badimport(): void {
    $json = '{"example_uint64":897234,"example_float":78575.334}';
    $obj = new Valid();
    $obj->import()->fromJSON($json);

    $testStringValue = 'another_string_value_80';
    $testIntValue = 897234;
    $testFloatValue = 78575.334;

    $this->assertEquals($testStringValue, $obj->example_string->get());
    $this->assertEquals($testIntValue, $obj->example_uint64->get());
    $this->assertEquals($testFloatValue, $obj->example_float->get());

  }

  public function testValidNested_ImportFromMap(): void {

    $otherVarValue = 'iCanNest';
    $exampleStringValue = 'validChildObjectString';
    $exampleUInt64Value = 123985;
    $exampleFloatValue = 12938.34;

    // simulate a map convered from a json decode.
    $testMap = Map {};

    $testMap['otherVar'] = $otherVarValue;
    $testMap['validObj'] = array(
      'example_string' => $exampleStringValue,
      'example_uint64' => $exampleUInt64Value,
      'example_float' => $exampleFloatValue,
    );

    $obj = new ValidNested();
    $this->assertTrue($obj->import()->fromMap($testMap));

  }

}
