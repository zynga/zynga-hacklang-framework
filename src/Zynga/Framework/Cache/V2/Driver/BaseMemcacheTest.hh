<?hh //strict

namespace Zynga\Framework\Cache\V2\Driver;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Cache\V2\Interfaces\DriverInterface;
use Zynga\Framework\Cache\V2\Driver\Memcache as MemcacheDriver;
use Zynga\Framework\Cache\V2\Exceptions\NoServerPairsProvidedException;

use Zynga\Framework\Exception\V1\Exception;

use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidStorableObject
;
use Zynga\Framework\Cache\V2\Exceptions\StorableObjectRequiredException;

abstract class BaseMemcacheTest extends TestCase {
  private float $testFloat = 3.14597;
  private int $testInt = 678912;
  private string $testString = 'how now brown cow';
  private ?ValidStorableObject $validStorable = null;

  public function doSetUpBeforeClass(): bool {
    parent::doSetUpBeforeClass();
    CacheFactory::disableMockDrivers();
    CacheFactory::clear();
    return true;
  }

  abstract public function getDriverName(): string;
  abstract public function getNonstorableDriverName(): string;
  abstract public function getNoServersConfiguredDriverName(): string;

  public function testGetConfigReturnsValidInstance(): void {
    $obj =
      CacheFactory::factory(DriverInterface::class, $this->getDriverName());
    $this->assertTrue($obj->getConfig() instanceof DriverConfigInterface);
  }

  public function testCanConnectToMemcache(): void {

    $obj =
      CacheFactory::factory(DriverInterface::class, $this->getDriverName());

    if ($obj instanceof MemcacheDriver) {
      // do a initial connection.
      $this->assertTrue($obj->connect());
      // test that the caching is working
      $this->assertTrue($obj->connect());
    }

  }

  public function testInvalidConnectionConfigThrowsException(): void {
    $obj = CacheFactory::factory(
      DriverInterface::class,
      $this->getNoServersConfiguredDriverName(),
    );

    if ($obj instanceof MemcacheDriver) {
      $this->expectException(NoServerPairsProvidedException::class);
      $obj->connect();
      return;
    }
    throw new Exception(
      'test='.
      __METHOD__.
      ' not able to be executed. Check the config was setup with correct driver type. driver='.
      $this->getNoServersConfiguredDriverName(),
    );
  }

  public function testCanSetAndGetStorable(): void {
    $obj = $this->getTestModel();

    $cache =
      CacheFactory::factory(DriverInterface::class, $this->getDriverName());

    // save the object into the cache.
    $this->assertTrue($cache->set($obj));

    // try to pull it back out of the cache
    $back = new ValidStorableObject();
    $back->example_uint64->set($this->testInt);

    $back = $cache->get($back);

    if ($back instanceof ValidStorableObject) {
      $this->assertEquals($this->testFloat, $back->example_float->get());
      $this->assertEquals($this->testString, $back->example_string->get());
    }
  }

  public function testCanGetByMap(): void {
    $obj = $this->getTestModel();

    $cache =
      CacheFactory::factory(DriverInterface::class, $this->getDriverName());

    // try to pull it via the map interface
    $cache->set($obj);
    $other = $cache->getByMap(Map {'example_uint64' => $this->testInt});

    if ($other instanceof ValidStorableObject) {
      $this->assertEquals($this->testInt, $other->example_uint64->get());
      $this->assertEquals($this->testFloat, $other->example_float->get());
      $this->assertEquals($this->testString, $other->example_string->get());

      // now remove the thing we just put in.
      $cache->delete($other);
    }
  }

  public function testCanDeleteByStorable(): void {
    $obj = $this->getTestModel();

    $cache =
      CacheFactory::factory(DriverInterface::class, $this->getDriverName());

    // try to pull it via the map interface
    $cache->set($obj);

    // now remove the thing we just put in.
    $this->assertTrue($cache->delete($obj));

    $this->assertEquals(null, $cache->get($obj));
  }

  public function testCanDeleteByMap(): void {
    $obj = $this->getTestModel();
    $cache =
      CacheFactory::factory(DriverInterface::class, $this->getDriverName());

    // now reinject the object back into the cache
    $cache->set($obj);
    $this->assertTrue(
      $cache->deleteByMap(Map {'example_uint64' => $this->testInt}),
    );
    $this->assertEquals(
      null,
      $cache->getByMap(Map {'example_uint64' => $this->testInt}),
    );
  }

  protected function getTestModel(): ValidStorableObject {
    if ($this->validStorable === null) {
      $obj = new ValidStorableObject();
      $obj->example_float->set($this->testFloat);
      $obj->example_string->set($this->testString);
      $obj->example_uint64->set($this->testInt);

      $this->validStorable = $obj;
    }

    return $this->validStorable;
  }

  public function testGetWithInvalidKeyThrowsException(): void {
    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $cache =
      CacheFactory::factory(DriverInterface::class, $this->getDriverName());

    $this->expectException(Exception::class);
    $cache->get($obj);
  }

  public function testSetWithInvalidKeyThrowsException(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $cache =
      CacheFactory::factory(DriverInterface::class, $this->getDriverName());
    $this->expectException(Exception::class);
    $cache->set($obj);
  }

  public function testDeleteWithInvalidKeyThrowsException(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $cache =
      CacheFactory::factory(DriverInterface::class, $this->getDriverName());
    $this->expectException(Exception::class);
    $cache->delete($obj);

  }

  public function testGetWithNonStorableThrowsException(): void {
    $cache = CacheFactory::factory(
      DriverInterface::class,
      $this->getNonstorableDriverName(),
    );
    $this->expectException(StorableObjectRequiredException::class);
    $cache->getByMap(Map {'foo' => 'bar'});
  }

  public function testDeleteByMapWithNonStorableThrowsException(): void {
    $cache = CacheFactory::factory(
      DriverInterface::class,
      $this->getNonstorableDriverName(),
    );
    $this->expectException(StorableObjectRequiredException::class);
    $cache->deleteByMap(Map {'foo' => 'bar'});
  }

}
