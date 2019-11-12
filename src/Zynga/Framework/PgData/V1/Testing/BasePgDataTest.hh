<?hh //strict

namespace Zynga\Framework\PgData\V1\Testing;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Cache\V2\Driver\InMemory;
use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Driver\Mock as MockDriver;
use Zynga\Framework\Lockable\Cache\V1\Factory as LockableCacheFactory;
use Zynga\Framework\ShardedDatabase\V3\Driver\Mock as MockShardedDriver;
use Zynga\Framework\ShardedDatabase\V3\Factory as ShardedDatabaseFactory;

abstract class BasePgDataTest extends TestCase {

  <<__Override>>
  public function setUp(): void {
    parent::setUp();
    CacheFactory::enableMockDrivers();
    LockableCacheFactory::enableMockDrivers();
    LockableCacheFactory::overrideMockDriver(
      'PgResultSet',
      'Mock\PgResultSet',
    );
    LockableCacheFactory::overrideMockDriver('PgData', 'Mock\PgData');

    DatabaseFactory::enableMockDrivers();
    DatabaseFactory::clear();
    ShardedDatabaseFactory::enableMockDrivers();
    ShardedDatabaseFactory::clear();
    $this->clearPGDataCaches();
  }

  <<__Override>>
  public function tearDown(): void {
    $this->clearPGDataCaches();
    LockableCacheFactory::disableMockDrivers();
    DatabaseFactory::clear();
    DatabaseFactory::disableMockDrivers();
    ShardedDatabaseFactory::clear();
    ShardedDatabaseFactory::disableMockDrivers();
    parent::tearDown();
  }

  protected function addEmptyResultSetToNonShardedDatabase(): void {
    $driver = DatabaseFactory::factory(MockDriver::class, 'Mock');
    $driver->addEmptyResultSet();
  }

  /**
   * @param string $file the name of the file to load, the suffix .data will be appended
   */
  protected function addResultSetsToNonShardedDatabaseFromFile(
    string $className,
    string $file,
  ): void {
    $this->assertTrue(
      DatabaseFactory::loadResultsForTest($className, $file),
      'Failed to load result sets',
    );
  }

  protected function addEmptyResultSetToShardedDatabase(): void {
    $driver =
      ShardedDatabaseFactory::factory(MockShardedDriver::class, 'Mock');
    $driver->addEmptyResultSet();
  }

  /**
   * @param string $file the name of the file to load, the suffix _UserSharded.data will be appended
   */
  protected function addResultSetsToShardedDatabaseFromFile(
    string $className,
    string $file,
  ): void {
    $this->assertTrue(
      ShardedDatabaseFactory::loadResultsForTest($className, $file),
      'Failed to load result sets',
    );
  }

  private function clearPGDataCaches(): void {
    $cacheDriver =
      CacheFactory::factory(InMemory::class, 'Mock\InMemory\PgResultSet');
    $cacheDriver->clearInMemoryCache();

    $cacheDriver = CacheFactory::factory(
      InMemory::class,
      'Mock\InMemory\PgResultSetLocks',
    );
    $cacheDriver->clearInMemoryCache();

    LockableCacheFactory::clear();
  }
}
