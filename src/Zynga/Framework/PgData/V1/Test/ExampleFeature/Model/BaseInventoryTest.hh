<?hh // strict

namespace Zynga\Framework\PgData\V1\Test\ExampleFeature\Model;

use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Lockable\Cache\V1\Factory as LockableCacheFactory;
use
  Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface as LockableCacheDriverInterface
;
use Zynga\Framework\PgData\V1\Exceptions\InvalidPrimaryKeyValueException;
use Zynga\Framework\PgData\V1\Interfaces\PgWhereClauseInterface;
use Zynga\Framework\PgData\V1\PgModel;
use Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\InventoryModel;
use Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\Inventory\ItemType;
use Zynga\Framework\PgData\V1\PgWhereClause;
use Zynga\Framework\PgData\V1\PgWhereOperand;

class BaseInventoryTest extends TestCase {

  public function doSetUpBeforeClass(): bool {

    parent::doSetUpBeforeClass();

    CacheFactory::disableMockDrivers();
    CacheFactory::clear();

    DatabaseFactory::disableMockDrivers();
    DatabaseFactory::clear();

    LockableCacheFactory::disableMockDrivers();
    LockableCacheFactory::clear();

    return true;

  }

  public function doTearDownAfterClass(): bool {

    CacheFactory::enableMockDrivers();
    CacheFactory::clear();

    DatabaseFactory::enableMockDrivers();
    DatabaseFactory::clear();

    LockableCacheFactory::enableMockDrivers();
    LockableCacheFactory::clear();

    return true;
  }
  
  protected function removeCachedItem(int $id): void {

    $lmc = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    $inv = new InventoryModel();
    $obj = new ItemType($inv);
    $obj->id->set($id);

    $lmc->delete($obj);

  }

  protected function removeCachedResultSet(): void {

    $lmc = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

  }

  protected function validateModelStats(
    PgModel $model,
    int $cacheHits,
    int $cacheMisses,
    int $sqlSelectCount,
  ): void {
    $this->assertEquals(
      $cacheHits,
      $model->stats()->getCacheHits(),
      'cache_hits',
    );
    $this->assertEquals(
      $cacheMisses,
      $model->stats()->getCacheMisses(),
      'cache_misses',
    );
    $this->assertEquals(
      $sqlSelectCount,
      $model->stats()->getSqlSelects(),
      'sql_select_count',
    );
  }
  
}
  
