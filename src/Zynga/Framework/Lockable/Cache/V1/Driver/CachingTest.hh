<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1\Driver;

use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use Zynga\Framework\Lockable\Cache\V1\Factory as LockableCacheFactory;
use
  Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverConfigInterface as LockableCacheDriverConfigInterface
;
use
  Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface as LockableCacheDriverInterface
;
use Zynga\Framework\Lockable\Cache\V1\Test\Mock\SimpleStorable;
use Zynga\Framework\Lockable\Cache\V1\Test\Mock\PgDataExample;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\InventoryModel;
use Zynga\Framework\Cache\V2\Exceptions\InvalidObjectForKeyCreationException;
use Zynga\Framework\Lockable\Cache\V1\Driver\Caching as CachingDriver;
use Zynga\Framework\Lockable\Cache\V1\LockPayload;
use
  Zynga\Framework\Lockable\Cache\V1\Exceptions\UnableToEstablishLockException
;
use \Exception;

class CachingTest extends TestCase {
  private Vector<string> $_cacheNames = Vector {'Mock', 'PgDataTest'};

  public function doSetupBeforeClass(): bool {

    CacheFactory::disableMockDrivers();
    LockableCacheFactory::disableMockDrivers();

    CacheFactory::clear();
    LockableCacheFactory::clear();

    return true;

  }

  public function doTearDownAfterClass(): bool {

    CacheFactory::clear();
    LockableCacheFactory::clear();

    CacheFactory::enableMockDrivers();
    LockableCacheFactory::enableMockDrivers();

    return true;

  }

  public function testIsLockedByMyThread(): void {
    $pgDataCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    $mockId = '123456';
    $mockTextValue = 'this-is-a-pgdata-text-value-'.mt_rand();

    $inv = new InventoryModel();

    $pgMock = new PgDataExample($inv);
    $pgMock->id->set($mockId);
    $pgMock->name->set($mockTextValue);

    $this->assertTrue($pgDataCache->lock($pgMock));
    $this->assertTrue($pgDataCache->isLockedByMyThread($pgMock));
    // This will succeed on the same thread but fail on separate thead.
    $this->assertTrue($pgDataCache->lock($pgMock));

  }

  public function testDrivers_Load(): void {

    foreach ($this->_cacheNames as $cacheName) {

      $instance = LockableCacheFactory::factory(
        LockableCacheDriverInterface::class,
        $cacheName,
      );

      $this->assertInstanceOf(LockableCacheDriverInterface::class, $instance);

    }

  }

  public function testDrivers_ConfigsOK(): void {

    foreach ($this->_cacheNames as $cacheName) {

      $instance = LockableCacheFactory::factory(
        LockableCacheDriverInterface::class,
        $cacheName,
      );

      $config = $instance->getConfig();

      $this->assertInstanceOf(
        LockableCacheDriverConfigInterface::class,
        $config,
      );

    }

  }

  public function test_LockUnlock_Mock(): void {

    $mockCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'Mock',
    );

    $mockObj = new SimpleStorable();
    $mockObj->example_uint64->set(time() + mt_rand());

    // Lock the object
    $this->assertTrue($mockCache->lock($mockObj));
    // Should still be locked
    $this->assertTrue($mockCache->lock($mockObj));

    // Unlock the object
    $this->assertTrue($mockCache->unlock($mockObj));
    // We should be unlocked
    $this->assertTrue($mockCache->unlock($mockObj));

  }

  public function test_LockUnlock_pgData(): void {
    $pgDataCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    $inv = new InventoryModel();
    $pgMock = new PgDataExample($inv);
    $pgMock->id->set(time() + mt_rand());

    // lock the object
    $this->assertTrue($pgDataCache->lock($pgMock));

    // Shoudl still be locked
    $this->assertTrue($pgDataCache->lock($pgMock));

    // Unlock the object
    $this->assertTrue($pgDataCache->unlock($pgMock));

    // Should be unlocked
    $this->assertTrue($pgDataCache->unlock($pgMock));

  }

  public function test_GetSetDeleteCycle_Mock(): void {

    $mockCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'Mock',
    );

    $mockId = time() + mt_rand();
    $mockTextValue = 'this-is-a-mock-text-value-'.mt_rand();

    $mockObj = new SimpleStorable();
    $mockObj->example_uint64->set($mockId);
    $mockObj->example_string->set($mockTextValue);

    // should be null as the item doesn't exist
    $this->assertEquals(null, $mockCache->get($mockObj));

    // save to cache
    $this->assertTrue($mockCache->set($mockObj));

    $returnObj = $mockCache->get($mockObj);

    if ($returnObj instanceof SimpleStorable) {
      $this->assertEquals($mockId, $returnObj->example_uint64->get());
      $this->assertEquals($mockTextValue, $returnObj->example_string->get());
    } else {
      $this->fail(
        'We should of seen SimpleStorable as a result from mockCache->get',
      );
    }

    // delete from cache
    $this->assertTrue($mockCache->delete($mockObj));

    // Test that it was indeed deleted.
    $this->assertEquals(null, $mockCache->get($mockObj));

  }

  public function test_GetSetDeleteCycle_pgData(): void {

    $pgDataCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    $mockId = time() + mt_rand();
    $mockTextValue = 'this-is-a-pgdata-text-value-'.mt_rand();

    $inv = new InventoryModel();

    $pgMock = new PgDataExample($inv);
    $pgMock->id->set($mockId);
    $pgMock->name->set($mockTextValue);

    // should be null as the item doesn't exist
    $this->assertEquals(null, $pgDataCache->get($pgMock));

    // save to cache
    $this->assertTrue($pgDataCache->set($pgMock));

    $returnObj = $pgDataCache->get($pgMock);

    if ($returnObj instanceof PgDataExample) {
      $this->assertEquals($mockId, $returnObj->id->get());
      $this->assertEquals($mockTextValue, $returnObj->name->get());
    } else {
      $this->fail(
        'We should of seen SimpleStorable as a result from pgDataCache->get',
      );
    }

    // delete from cache
    $this->assertTrue($pgDataCache->delete($pgMock));

    // Test that it was indeed deleted.
    $this->assertEquals(null, $pgDataCache->get($pgMock));

  }

  public function test_LockCacheKeyFromStorableObject_FailureTrap(): void {

    LockableCacheFactory::clear();

    $pgDataCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    $obj = new SimpleStorable();

    $this->expectException(InvalidObjectForKeyCreationException::class);
    $pgDataCache->lock($obj);

  }

  public function test_lock_restablishIfExpired(): void {

    $pgDataCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    $mockId = time() + mt_rand();
    $mockTextValue = 'this-is-a-pgdata-text-value-'.mt_rand();

    $inv = new InventoryModel();

    $pgMock = new PgDataExample($inv);
    $pgMock->id->set($mockId);
    $pgMock->name->set($mockTextValue);

    // should be null as the item doesn't exist
    $this->assertEquals(null, $pgDataCache->get($pgMock));

    $this->assertTrue($pgDataCache->lock($pgMock));

    // We need to wind into the locks array and jiggle the lock to be already expired.
    if ($pgDataCache instanceof CachingDriver) {

      // we should only have 1 lock
      $locks = $pgDataCache->getActiveLocks();
      $this->assertEquals(1, $locks->count());

      $lockKey = $locks->firstKey();
      $lock = $locks->firstValue();

      if (is_string($lockKey) && $lock instanceof LockPayload) {

        // rewind the lock establishment in time.
        $lock->setLockEstablishment(0);

        // Remove the mc key so the restablish can happen.
        $pgDataCache->getConfig()->getLockCache()->delete($pgMock, $lockKey);

      } else {
        $this->fail('lock should be a LockPayload');
      }

    } else {
      $this->fail('pgDataCache should be a CachingDriver');
    }

    // attempt the lock again, which should restablish it.
    $this->assertTrue($pgDataCache->lock($pgMock));

    if ($pgDataCache instanceof CachingDriver) {

      $locks = $pgDataCache->getActiveLocks();

      $this->assertEquals(1, $locks->count());

      $lock = $locks->firstValue();

      if ($lock instanceof LockPayload) {
        $this->assertGreaterThan(0, $lock->getLockEstablishment());
      } else {
        $this->fail('lock should be a LockPayload');
      }

    }

  }

  public function test_lock_lockFailure(): void {

    LockableCacheFactory::clear();

    $pgDataCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    $mockId = time() + mt_rand();
    $mockTextValue = 'this-is-a-pgdata-text-value-'.mt_rand();

    $inv = new InventoryModel();

    $pgMock = new PgDataExample($inv);
    $pgMock->id->set($mockId);
    $pgMock->name->set($mockTextValue);

    // should be null as the item doesn't exist
    $this->assertEquals(null, $pgDataCache->get($pgMock));

    $this->assertTrue($pgDataCache->lock($pgMock));

    $lockKey = '';

    // We need to wind into the locks array and jiggle the lock to be already expired.
    if ($pgDataCache instanceof CachingDriver) {

      // we should only have 1 lock
      $locks = $pgDataCache->getActiveLocks();
      $this->assertEquals(1, $locks->count());

      $lockKey = $locks->firstKey();
      $lock = $locks->firstValue();

      if (is_string($lockKey) && $lock instanceof LockPayload) {

        // rewind the lock establishment in time.
        $lock->setLockEstablishment(0);

      } else {
        $this->fail('lock should be a LockPayload');
      }

    } else {
      $this->fail('pgDataCache should be a CachingDriver');
    }

    // attempt the lock again, which should restablish it.
    $this->assertFalse($pgDataCache->lock($pgMock));

    // Remove the mc key to cleanup
    if (is_string($lockKey)) {
      $pgDataCache->getConfig()->getLockCache()->delete($pgMock, $lockKey);
    }

  }

  public function test_UnlockCacheKeyFromStorableObject_FailureTrap(): void {

    LockableCacheFactory::clear();

    $pgDataCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    $obj = new SimpleStorable();

    $this->expectException(InvalidObjectForKeyCreationException::class);
    $pgDataCache->unlock($obj);

  }

  public function test_unlock_Failure(): void {

    LockableCacheFactory::clear();

    $pgDataCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    $mockId = time() + mt_rand();
    $mockTextValue = 'this-is-a-pgdata-text-value-'.mt_rand();

    $inv = new InventoryModel();

    $pgMock = new PgDataExample($inv);
    $pgMock->id->set($mockId);
    $pgMock->name->set($mockTextValue);

    // should be null as the item doesn't exist
    $this->assertEquals(null, $pgDataCache->get($pgMock));

    $this->assertTrue($pgDataCache->lock($pgMock));

    $lockKey = '';

    // We need to wind into the locks array and jiggle the lock to be already expired.
    if ($pgDataCache instanceof CachingDriver) {

      // we should only have 1 lock
      $locks = $pgDataCache->getActiveLocks();
      $this->assertEquals(1, $locks->count());

      $lockKey = $locks->firstKey();
      $lock = $locks->firstValue();

      if (is_string($lockKey) && $lock instanceof LockPayload) {

        $pgDataCache->getConfig()->getLockCache()->delete($pgMock, $lockKey);

      } else {
        $this->fail('lock should be a LockPayload');
      }

    } else {
      $this->fail('pgDataCache should be a CachingDriver');
    }

    // unlock the object, but due to our mc muckery it should fail.
    $this->assertFalse($pgDataCache->unlock($pgMock));

  }

  public function test_get_lockFailure(): void {

    LockableCacheFactory::clear();

    $pgDataCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    $mockId = time() + mt_rand();
    $mockTextValue = 'this-is-a-pgdata-text-value-'.mt_rand();

    $inv = new InventoryModel();

    $pgMock = new PgDataExample($inv);
    $pgMock->id->set($mockId);
    $pgMock->name->set($mockTextValue);

    // Establish a lock
    $pgDataCache->lock($pgMock);

    // Now toss away our knowledge of the lock.
    LockableCacheFactory::clear();

    // grab a new instance
    $pgDataCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    $this->expectException(UnableToEstablishLockException::class);
    $pgDataCache->get($pgMock, true);

  }

  public function test_set_Failure(): void {

    LockableCacheFactory::clear();

    $pgDataCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    $mockId = time() + mt_rand();
    $mockTextValue = 'this-is-a-pgdata-text-value-'.mt_rand();

    $inv = new InventoryModel();

    $pgMock = new PgDataExample($inv);
    $pgMock->id->set($mockId);
    $pgMock->name->set($mockTextValue);

    // should be null as the item doesn't exist
    $this->assertEquals(null, $pgDataCache->get($pgMock));

    $this->assertTrue($pgDataCache->lock($pgMock));

    // toss away our knowledge of the lock.
    LockableCacheFactory::clear();

    $pgDataCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    // set the object, but due to our mc muckery it should fail.
    $this->expectException(UnableToEstablishLockException::class);
    $pgDataCache->set($pgMock);

  }

  public function test_delete_Failure(): void {

    LockableCacheFactory::clear();

    $pgDataCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    $mockId = time() + mt_rand();
    $mockTextValue = 'this-is-a-pgdata-text-value-'.mt_rand();

    $inv = new InventoryModel();

    $pgMock = new PgDataExample($inv);
    $pgMock->id->set($mockId);
    $pgMock->name->set($mockTextValue);

    // should be null as the item doesn't exist
    $this->assertEquals(null, $pgDataCache->get($pgMock));

    $this->assertTrue($pgDataCache->lock($pgMock));

    // toss away our knowledge of the lock.
    LockableCacheFactory::clear();

    $pgDataCache = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    // set the object, but due to our mc muckery it should fail.
    $this->expectException(UnableToEstablishLockException::class);
    $pgDataCache->delete($pgMock);

  }

}
