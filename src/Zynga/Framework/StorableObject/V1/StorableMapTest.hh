<?hh //strict

namespace Zynga\Framework\StorableObject\V1;

use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;
use Zynga\Framework\StorableObject\V1\StorableMap;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Nofields as NofieldsStorableObject
;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidStorableObject;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\UInt64Box;

class StorableMapTest extends TestCase {

  public function test_valid_simple_entry(): void {
    $randomValue = mt_rand(1, 1000000);

    $testKey = 'a-key';

    $testValue = new UInt64Box();
    $testValue->set($randomValue);

    $map = new StorableMap();

    list($isDefaultValue, $defaultFields) = $map->isDefaultValue();
    $this->assertTrue($isDefaultValue);

    // add a item to the map
    $this->assertTrue($map->set($testKey, $testValue));

    // see if the item actually stored to the map
    $testObject = $map->get($testKey);

    $this->assertEquals(UInt64Box::class, get_class($testObject));

    if ($testObject instanceof UInt64Box) {
      $this->assertEquals($randomValue, $testObject->get());
    }

    list($isDefaultValue, $defaultFields) = $map->isDefaultValue();
    $this->assertFalse($isDefaultValue);

    $mapData = $map->getMap();
    $this->assertEquals(1, $map->count());
    $this->assertEquals(1, $mapData->count());
  }

  public function test_empty_map(): void {
    // make sure we return null when asking for something that isn't there.
    $map = new StorableMap();
    $someOtherKey = 'some-other-key';
    $this->assertEquals(null, $map->get($someOtherKey));
  }

  public function test_isRequiredFlag(): void {
    $map = new StorableMap();

    $this->assertFalse($map->getIsRequired());
    $this->assertTrue($map->setIsRequired(true));
    $this->assertTrue($map->getIsRequired());
  }

  public function test_isDefaultValueFlag(): void {
    $map = new StorableMap();

    // currently this is a empty default value.
    list($isDefaultValue, $defaultValues) = $map->isDefaultValue();
    $this->assertTrue($isDefaultValue);

    // add a key to the map
    $randomValue = mt_rand(1, 1000000);

    $testKey = 'a-key';

    $testValue = new UInt64Box();
    $testValue->set($randomValue);

    $this->assertTrue($map->set($testKey, $testValue));

    // Should be false because the map now has a entry
    list($isDefaultValue, $defaultValues) = $map->isDefaultValue();
    $this->assertFalse($isDefaultValue);

    // now add another value and check for default values from within.
    $anotherValue = new UInt64Box();
    $anotherTestKey = 'another-key';

    $this->assertTrue($map->set($anotherTestKey, $anotherValue));

    // now it should be not default, and with a report that another-key is in a default state.
    list($isDefaultValue, $defaultValues) = $map->isDefaultValue();
    $this->assertFalse($isDefaultValue);
    //file_put_contents('/tmp/jeo-needs-data.txt', print_r($isDefaultValue, true));
    $this->assertEquals(1, $defaultValues->count());

    // now let's wreck some stuff ;)
    // $this->assertTrue($map->setIsDefaultValue(true));
    // list($isDefaultValue, $defaultValues) = $map->isDefaultValue();
    // $this->assertTrue($isDefaultValue);
    // $this->assertEquals(2, $map->count());
  }

  public function test_std_interface(): void {
    $map = new StorableMap();
    $this->assertTrue($map->fields() instanceof FieldsInterface);
    $this->assertTrue($map->import() instanceof ImportInterface);
    $this->assertTrue($map->export() instanceof ExportInterface);
  }

  public function testReset(): void {
    $map = new StorableMap();
    $this->assertTrue($map->reset());
  }
}
