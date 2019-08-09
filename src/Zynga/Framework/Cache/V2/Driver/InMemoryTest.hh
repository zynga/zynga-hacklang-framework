<?hh //strict

namespace Zynga\Framework\Cache\V2\Driver;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Cache\V2\Driver\InMemory as InMemoryDriver;

use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidStorableObject
;
use Zynga\Framework\Exception\V1\Exception;

class InMemoryTest extends TestCase {

  public function doSetUpBeforeClass(): bool {

    parent::doSetUpBeforeClass();

    CacheFactory::disableMockDrivers();

    CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');

    return true;

  }

  <<__Override>>
  public function tearDown(): void {
    parent::tearDown();
    CacheFactory::clear();
  }

  public static function driverProvider(): InMemoryDriver {

    $obj = CacheFactory::factory(InMemoryDriver::class, 'InMemory_Mock');
    $obj->clearInMemoryCache();

    return $obj;

  }

  public static function invalidKeyConditionProvider(): array<array<mixed>> {

    $ret = array(array(self::driverProvider(), new ValidStorableObject()));

    return $ret;

  }

  <<dataProvider("driverProvider")>>
  public function testGetConfig(InMemoryDriver $obj): void {
    $this->assertInstanceOf(DriverConfigInterface::class, $obj->getConfig());
  }

  <<dataProvider("driverProvider")>>
  public function testValidCycle(InMemoryDriver $cache): void {

    $testFloat = 3.14597;
    $testString = 'how now brown cow';
    $testInt = 678912;

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $obj->example_float->set($testFloat);
    $obj->example_string->set($testString);
    $obj->example_uint64->set($testInt);

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

  <<dataProvider("invalidkeyConditionProvider")>>
  public function testGet_InvalidKeyCondition(
    InMemoryDriver $cache,
    ValidStorableObject $obj,
  ): void {
    $this->expectException(Exception::class);
    $cache->get($obj);
  }

  <<dataProvider("invalidkeyConditionProvider")>>
  public function testSet_InvalidKeyCondition(
    InMemoryDriver $cache,
    ValidStorableObject $obj,
  ): void {
    $this->expectException(Exception::class);
    $cache->set($obj);

  }

  <<
  dataProvider("driverProvider"),
  expectedException("Zynga\Framework\Exception\V1\Exception")
  >>
  public function testDelete_InvalidKeyCondition(InMemoryDriver $cache): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $cache->delete($obj);

  }

  <<dataProvider("invalidkeyConditionProvider")>>
  public function testAdd_InvalidKeyCondition(
    InMemoryDriver $cache,
    ValidStorableObject $obj,
  ): void {

    $this->expectException(Exception::class);
    $cache->add($obj);

  }

  <<dataProvider("driverProvider")>>
  public function testAdd_Valid(InMemoryDriver $cache): void {

    // stand up a normal, valid object.
    $obj = new ValidStorableObject();
    $obj->example_uint64->set(120330908743);

    // add the object into the cache.
    $this->assertTrue($cache->add($obj));

    // if this item is laready in the cache, it shouldn't be.
    $this->assertFalse($cache->add($obj));

  }

  <<dataProvider("driverProvider")>>
  public function testGet_NotSetYet(InMemoryDriver $cache): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $obj->example_uint64->set(12989745);
    $this->assertEquals(null, $cache->get($obj));

  }

  <<dataProvider("driverProvider")>>
  public function testDelete_Valid(InMemoryDriver $cache): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $obj->example_uint64->set(12989745);

    $this->assertTrue($cache->set($obj));
    $this->assertTrue($cache->delete($obj));

  }

  <<dataProvider("driverProvider")>>
  public function testDelete_NotSetYet(InMemoryDriver $cache): void {

    // stand up a empty storable object
    $obj = new ValidStorableObject();
    $obj->example_uint64->set(12989745);

    $this->assertFalse($cache->delete($obj));

  }

  <<dataProvider("driverProvider")>>
  public function testClearInMemoryCacheWorks(InMemoryDriver $cache): void {
    $this->assertTrue($cache->clearInMemoryCache());
  }

  <<dataProvider("driverProvider")>>
  public function testConnect(InMemoryDriver $cache): void {
    $this->assertTrue($cache->connect());
  }

  <<dataProvider("driverProvider")>>
  public function testDirectIncrementFails(InMemoryDriver $cache): void {
    $cache->directDelete('test');
    $this->assertEquals(0, $cache->directIncrement('test', 1));
  }

  <<dataProvider("driverProvider")>>
  public function testDirectIncrementIncrements(InMemoryDriver $cache): void {
    $cache->directAdd('test', 1);
    $this->assertEquals(2, $cache->directIncrement('test', 1));
    $this->assertTrue($cache->directDelete('test'));
  }

  <<dataProvider("driverProvider")>>
  public function testDirectAddAndDeleteSucceeds(InMemoryDriver $cache): void {
    $this->assertTrue($cache->directAdd('test', 1));
    $this->assertFalse($cache->directAdd('test', 1));
    $this->assertTrue($cache->directDelete('test'));
  }

  <<dataProvider("driverProvider")>>
  public function testDirectSetOverwritesPreviousValue(
    InMemoryDriver $cache,
  ): void {

    $this->assertTrue($cache->directSet('test', 1));
    $this->assertEquals(1, $cache->directGet('test'));
    // overwrite the previous value
    $this->assertTrue($cache->directSet('test', 2));
    $this->assertEquals(2, $cache->directGet('test'));

    $this->assertTrue($cache->directDelete('test'));
  }

  <<dataProvider("driverProvider")>>
  public function testDirectGetReturnsExpectedValueAfterAdd(
    InMemoryDriver $cache,
  ): void {
    $this->assertTrue($cache->directAdd('test', 'value'));
    $this->assertEquals('value', $cache->directGet('test'));
    $this->assertTrue($cache->directDelete('test'));
  }

  <<dataProvider("driverProvider")>>
  public function testDirectGetReturnsNullForNonexistentKey(
    InMemoryDriver $cache,
  ): void {
    $this->assertEquals(null, $cache->directGet('test'));
  }

  <<dataProvider("driverProvider")>>
  public function testDirectDeleteFails(InMemoryDriver $cache): void {
    $this->assertFalse($cache->directDelete('test'));
  }

}
