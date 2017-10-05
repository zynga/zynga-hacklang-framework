<?hh //strict

namespace Zynga\Framework\StorableObject\V1\StoreableMap;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\StorableObject\V1\StorableMap;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidStorableObject;
use Zynga\Framework\StorableObject\V1\Test\Mock\Nofields as NofieldsStorableObject;
use Zynga\Framework\StorableObject\V1\Test\Mock\Broken\ValidButBrokenExporter;

use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\Int64Box;

class ExporterTest extends TestCase {

  public function test_asJSON(): void {

    $testValue = 'a-weird-string-value';
    $testFloat = 32948.34;
    $testInt = 30984;

    $stringBox = new StringBox();
    $stringBox->set($testValue);

    $stoObject = new ValidStorableObject();
    $stoObject->example_float->set($testFloat);
    $stoObject->example_string->set($testValue);
    $stoObject->example_uint64->set($testInt);

    $noFieldsObject = new NofieldsStorableObject();

    $map = new StorableMap();
    $map->set('test-string-box', $stringBox);
    $map->set('test-storable', $stoObject);
    $map->set('no-fields', $noFieldsObject);

    $targetValue =
      '{"test-string-box":"a-weird-string-value","test-storable":{"example_string":"a-weird-string-value","example_uint64":30984,"example_float":32948.34}}';
    $this->assertEquals($targetValue, $map->export()->asJSON());

  }

  public function test_asJSON_StringWithInt64Box(): void {

    $data1 = new Int64Box();
    $data1->set(3248748755);

    $data2 = new Int64Box();
    $data2->set(0);

    $map = new StorableMap();
    $map->set('testing-storing', $data1);
    $map->set('testing-storing-2', $data1);

    $targetValue = '{"testing-storing":3248748755,"testing-storing-2":3248748755}';
    $this->assertEquals($targetValue, $map->export()->asJSON());


  }


  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException
   */
  public function test_asJSON_noFields(): void {
    $map = new StorableMap();
    $map->export()->asJSON();
  }

  public function test_asJSON_recursiveStorable(): void {
    $map = new StorableMap();
    $obj = new ValidStorableObject();
    $this->assertTrue($map->set('a-key', $obj));

    // Test that since we didn't set any values this should collapse to a empty array.
    $targetJson = '{}';
    $this->assertEquals($targetJson, $map->export()->asJSON());

    $someString = 'how now brown cow?';
    $someUint64 = 238472329834;
    $someFloat = 1234.45;

    $anotherObj = new ValidStorableObject();
    $anotherObj->example_string->set($someString);
    $anotherObj->example_uint64->set($someUint64);
    $anotherObj->example_float->set($someFloat);

    $this->assertTrue($map->set('another-key', $anotherObj));

    $targetJson =
      '{"another-key":{"example_string":"'.
      $someString.
      '","example_uint64":'.
      $someUint64.
      ',"example_float":'.
      $someFloat.
      '}}';
    $this->assertEquals($targetJson, $map->export()->asJSON());
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function test_asJSON_brokenExporter(): void {

    $obj = new ValidButBrokenExporter();
    $obj->example_float->set(3.145);

    $map = new StorableMap();
    $map->set('kaboom', $obj);

    $map->export()->asJSON();

  }

  public function test_asJSON_emptyChild(): void {

    $map = new StorableMap();
    $map->setIsDefaultValue(false);

    $obj = new StorableMap();
    $obj->setIsDefaultValue(false);

    $this->assertTrue($map->set('a-key-emptyChild', $obj));

    $json = $map->export()->asJSON();
    $this->assertEquals('{}', $json);
  }

  public function test_asMap_noFields(): void {
    $map = new StorableMap();
    $mapData = $map->export()->asMap();
    $this->assertEquals(0, $mapData->count());
  }

  public function test_asMap_childType(): void {

    $testValue = 'a-weird-string-value';

    $stringBox = new StringBox();
    $stringBox->set($testValue);

    $obj = new StorableMap();
    $obj->set('a-key', $stringBox);

    $map = $obj->export()->asMap();

    $this->assertEquals(1, $map->count());

  }

  public function test_asMap_recursive(): void {

    $map = new StorableMap();
    $obj = new ValidStorableObject();
    $this->assertTrue($map->set('a-key', $obj));

    $targetMap = Map {};
    $targetMap['a-key'] = Map {
      'example_string' => '',
      'example_uint64' => 0,
      'example_float' => 0,
    };

    $this->assertEquals($targetMap, $map->export()->asMap());

  }

  /**
   * @expectedException Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException
   */
  public function test_asMap_noFieldsOnChild(): void {
    $map = new StorableMap();
    $obj = new NofieldsStorableObject();
    $this->assertTrue($map->set('a-key', $obj));
    $map->export()->asMap();
  }

  public function test_asBinary(): void {
    $map = new StorableMap();
    $this->assertEquals('', $map->export()->asBinary());
  }

}
