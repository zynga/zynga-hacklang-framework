<?hh //strict

namespace Zynga\Framework\Cache\V2\Driver;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Cache\V2\Driver\InMemory as InMemoryDriver;

use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidStorableObject
;

class InMemoryTest extends TestCase {

  public function doSetUpBeforeClass(): bool {
    parent::doSetUpBeforeClass();
    CacheFactory::disableMockDrivers();
    return true;
  }

  <<__Override>>
  public function tearDown(): void {
    parent::tearDown();
    CacheFactory::clear();
  }

  public function testGetConfig(): void {

    $obj = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');
    $this->assertTrue($obj->getConfig() instanceof DriverConfigInterface);

  }

  public function testValidCycle(): void {

    $testFloat = 3.14597;
    $testString = 'how now brown cow';
    $testInt = 678912;

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $obj->example_float->set($testFloat);
    $obj->example_string->set($testString);
    $obj->example_uint64->set($testInt);

    $cache = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');

    // save the object into the cache.
    $this->assertTrue($cache->set($obj));

    // try to pull it back out of the cache
    $back = new ValidStorableObject();
    $back->example_uint64->set($testInt);

    $back = $cache->get($back);

    if ($back instanceof ValidStorableObject) {
      $this->assertEquals($testFloat, $back->example_float->get());
      $this->assertEquals($testString, $back->example_string->get());
    }

    // now reinject the object back into the cache
    $this->assertTrue($cache->set($obj));

    $again = $cache->get($obj);

    if ($again instanceof ValidStorableObject) {

      $this->assertTrue(true);

    }

  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function testGet_InvalidKeyCondition(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $cache = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');
    $cache->get($obj);

  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function testSet_InvalidKeyCondition(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $cache = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');
    $cache->set($obj);

  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function testDelete_InvalidKeyCondition(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $cache = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');
    $cache->delete($obj);

  }

  public function testAdd_Valid(): void {

    // stand up a normal, valid object.
    $obj = new ValidStorableObject();
    $obj->example_uint64->set(120330908743);

    $cache = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');

    // add the object into the cache.
    $this->assertTrue($cache->add($obj));

    // if this item is laready in the cache, it shouldn't be.
    $this->assertFalse($cache->add($obj));

  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function testAdd_InvalidKeyCondition(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $cache = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');
    $cache->add($obj);

  }

  public function testGet_NotSetYet(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $obj->example_uint64->set(12989745);
    $cache = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');
    $this->assertEquals(null, $cache->get($obj));

  }

  public function testDelete_Valid(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $obj->example_uint64->set(12989745);
    $cache = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');
    $this->assertTrue($cache->set($obj));
    $this->assertTrue($cache->delete($obj));

  }

  public function testDelete_NotSetYet(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $obj->example_uint64->set(12989745);
    $cache = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');
    $this->assertFalse($cache->delete($obj));

  }
  
  public function testClearInMemoryCacheWorks(): void {
    $cache = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');
    $this->assertTrue($cache->clearInMemoryCache());
  }
  
  public function testConnect(): void {
    $cache = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');
    $this->assertTrue($cache->connect());
  }
  
  public function testDirectIncrementFails(): void {
    $cache = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');
    $cache->directDelete('test');
    $this->assertEquals(0, $cache->directIncrement('test', 1));
  }
  
  public function testDirectIncrementIncrements(): void {
    $cache = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');
    $cache->clearInMemoryCache();
    $cache->directAdd('test', 1);
    $this->assertEquals(2, $cache->directIncrement('test', 1));
    $this->assertTrue($cache->directDelete('test'));
  }
  
  public function testDirectAddAndDeleteSucceeds(): void {
    $cache = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');
    $cache->clearInMemoryCache();
    $this->assertTrue($cache->directAdd('test', 1));
    $this->assertFalse($cache->directAdd('test', 1));
    $this->assertTrue($cache->directDelete('test'));
  }
  
  public function testDirectDeleteFails(): void {
    $cache = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');
    $this->assertFalse($cache->directDelete('test'));
  }
}
