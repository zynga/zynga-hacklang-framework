<?hh //strict

namespace Zynga\Framework\Cache\V2\Driver;

use Zynga\Framework\Cache\V2\Driver\Memcache as MemcacheDriver;
use Zynga\Framework\Cache\V2\Exceptions\CacheDoesNotSupportKeyOverride;
use Zynga\Framework\Cache\V2\Exceptions\CacheDoesNotSupportTTLOverride;
use Zynga\Framework\Cache\V2\Exceptions\CacheRequiresTTLException;
use Zynga\Framework\Cache\V2\Exceptions\CacheTTLExceededException;
use Zynga\Framework\Cache\V2\Exceptions\InvalidIncrementStepException;
use Zynga\Framework\Cache\V2\Exceptions\NoServerPairsProvidedException;
use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Exception\V1\Exception;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidStorableObject
;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class MemcacheTest extends TestCase {

  public function doSetUpBeforeClass(): bool {
    parent::doSetUpBeforeClass();
    CacheFactory::disableMockDrivers();
    return true;
  }

  public function testGetConfig(): void {

    $obj = CacheFactory::factory(MemcacheDriver::class, 'Mock');
    $this->assertTrue($obj->getConfig() instanceof DriverConfigInterface);

  }

  public function testConnect(): void {

    $obj = CacheFactory::factory(MemcacheDriver::class, 'Mock');

    if ($obj instanceof MemcacheDriver) {
      // do a initial connection.
      $this->assertTrue($obj->connect());
      // test that the caching is working
      $this->assertTrue($obj->connect());
    }

  }

  public function testConnect_NoServersConfigured(): void {

    $obj = CacheFactory::factory(
      MemcacheDriver::class,
      'Mock_NoServersConfigured',
    );

    if ($obj instanceof MemcacheDriver) {
      $this->expectException(NoServerPairsProvidedException::class);
      $obj->connect();
    }

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

    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');

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

  <<expectedException("Zynga\Framework\Exception\V1\Exception")>>
  public function testGet_InvalidKeyCondition(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');
    $cache->get($obj);

  }

  <<expectedException("Zynga\Framework\Exception\V1\Exception")>>
  public function testSet_InvalidKeyCondition(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');
    $cache->set($obj);

  }

  public function testDelete_InvalidKeyCondition(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');

    $this->expectException(Exception::class);
    $cache->delete($obj);

  }

  public function testAdd_Valid(): void {

    // stand up a normal, valid object.
    $obj = new ValidStorableObject();
    $obj->example_uint64->set(120330908743);

    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');

    // Purge any cache entries first.
    $cache->delete($obj);

    // add the object into the cache.
    $this->assertTrue($cache->add($obj));

    // if this item is laready in the cache, it shouldn't be.
    $this->assertFalse($cache->add($obj));

  }

  public function testAdd_InvalidKeyCondition(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');

    $this->expectException(Exception::class);
    $cache->add($obj);

  }

  public function testGet_NotSetYet(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $obj->example_uint64->set(12989745);
    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');
    $this->assertEquals(null, $cache->get($obj));

  }

  public function testDelete_NotSetYet(): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $obj->example_uint64->set(12989745);
    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');
    $this->assertFalse($cache->delete($obj));

  }

  public function testIncrement_BadIncrementValue(): void {
    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');
    $this->expectException(InvalidIncrementStepException::class);
    $cache->directIncrement('some-key-'.mt_rand(), 0);
  }

  public function testIncrement(): void {
    $cache = CacheFactory::factory(
      MemcacheDriver::class,
      'LocalMemcache_PgDataTest',
    );
    $this->assertEquals(
      0,
      $cache->directIncrement('inc-test-some-key-'.mt_rand()),
    );
  }

  public function testErrorTrap_DirectAdd(): void {
    $cache = CacheFactory::factory(
      MemcacheDriver::class,
      'Mock_NoServersConfigured',
    );
    $this->expectException(NoServerPairsProvidedException::class);
    $cache->directAdd('bc-test-some-key-'.mt_rand(), 'some-value-'.mt_rand());
  }

  public function testErrorTrap_DirectSet(): void {
    $cache = CacheFactory::factory(
      MemcacheDriver::class,
      'Mock_NoServersConfigured',
    );
    $this->expectException(NoServerPairsProvidedException::class);
    $cache->directSet('test-some-key-'.mt_rand(), 'some-value-'.mt_rand());
  }

  public function testErrorTrap_DirectGet(): void {
    $cache = CacheFactory::factory(
      MemcacheDriver::class,
      'Mock_NoServersConfigured',
    );
    $this->expectException(NoServerPairsProvidedException::class);
    $cache->directGet('test-some-key-'.mt_rand());
  }

  public function testErrorTrap_DirectDelete(): void {
    $cache = CacheFactory::factory(
      MemcacheDriver::class,
      'Mock_NoServersConfigured',
    );
    $this->expectException(NoServerPairsProvidedException::class);
    $cache->directDelete('bc-test-some-key-'.mt_rand());

  }

  public function testDirectDelete(): void {
    $cache = CacheFactory::factory(
      MemcacheDriver::class,
      'LocalMemcache_PgDataTest',
    );

    $randomKey = 'tdd-some-key-'.mt_rand();

    $this->assertFalse($cache->directDelete($randomKey));

    $this->assertTrue($cache->directAdd($randomKey, 1));

    $this->assertTrue($cache->directDelete($randomKey));

  }

  public function testDirectSetGetCycle(): void {
    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');

    $key = 'demo';
    $value = '{"test":"data"}';

    $this->assertTrue($cache->directSet($key, $value));
    $cachedValue = $cache->directGet($key);

    $this->assertEquals($value, $cachedValue);
  }

  public function testCacheAllowsKeyOverride_Fail(): void {
    $cache =
      CacheFactory::factory(MemcacheDriver::class, 'Mock_NoCacheKeyOverride');
    $obj = new ValidStorableObject();
    $obj->example_uint64->set(12989745);
    $someOtherKey = 'cas-some-other-key-'.mt_rand();
    $this->expectException(CacheDoesNotSupportKeyOverride::class);
    $cache->getKeySupportingOverride($obj, $someOtherKey);
  }

  public function testCacheAllowsTTLOverride_Fail(): void {
    $cache =
      CacheFactory::factory(MemcacheDriver::class, 'Mock_NoCacheKeyOverride');
    $this->expectException(CacheDoesNotSupportTTLOverride::class);
    $cache->getTTLSupportingOverride(1234874);
  }

  public function testCacheRequiresTTLOverride_Fail(): void {
    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');
    $this->expectException(CacheRequiresTTLException::class);
    $cache->getTTLSupportingOverride(0);
  }

  public function testCacheExceedsTTLOverride_Fail(): void {
    $cache = CacheFactory::factory(MemcacheDriver::class, 'Mock');
    $ttl = $cache->getConfig()->getTTL();
    $ttl += 1;
    $this->expectException(CacheTTLExceededException::class);
    $cache->getTTLSupportingOverride($ttl);
  }

}
