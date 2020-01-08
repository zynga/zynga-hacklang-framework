<?hh //strict

namespace Zynga\Framework\Cache\V2\Config\Mock;

use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use Zynga\Framework\Cache\V2\Driver\InMemory as InMemoryDriver;
use Zynga\Framework\Cache\V2\Exceptions\CacheRequiresTTLException;
use Zynga\Framework\Cache\V2\Exceptions\InvalidObjectForKeyCreationException;
use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidStorableNoRequiredObject
;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidStorableObject;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

abstract class BaseTest extends TestCase {

  abstract public function createConfigUnderTest(): DriverConfigInterface;

  public function testCreateKeyFromStorableObject_NotTheRightStorable(): void {

    $obj = new ValidStorableObject();

    $config = $this->createConfigUnderTest();

    $this->expectException(InvalidObjectForKeyCreationException::class);
    $key = $config->createKeyFromStorableObject($obj);

  }

  public function testCreateKeyFromStorableObject_DefaultDataProvided(): void {

    $obj = new ValidStorableObject();

    $config = $this->createConfigUnderTest();

    $this->expectException(InvalidObjectForKeyCreationException::class);
    $key = $config->createKeyFromStorableObject($obj);

  }

  public function testCreateKeyFromStorableObject_valid(): void {

    $obj = new ValidStorableObject();
    $obj->example_string->set('monkey');
    $obj->example_float->set(12.0);

    $config = $this->createConfigUnderTest();
    $key = $config->createKeyFromStorableObject($obj);
    $this->assertEquals(
      'zfc:f652449a796e4010ea291230869a9976:91c064901d2905891402fc4dd1365d1b',
      $key,
    );

  }

}
