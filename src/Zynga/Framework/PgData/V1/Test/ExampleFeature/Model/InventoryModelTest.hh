<?hh // strict

namespace Zynga\Framework\PgData\V1\Test\ExampleFeature\Model;

use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Lockable\Cache\V1\Factory as LockableCacheFactory;
use
  Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface as LockableCacheDriverInterface
;
use Zynga\Framework\PgData\V1\PgModel;
use Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\InventoryModel;
use Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\Inventory\ItemType;

class InventoryModelTest extends TestCase {

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

  private function removeCachedItem(int $id): void {

    $lmc = LockableCacheFactory::factory(
      LockableCacheDriverInterface::class,
      'PgDataTest',
    );

    $obj = new ItemType();
    $obj->id->set($id);

    $lmc->delete($obj);

  }

  private function validateModelStats(
    PgModel $model,
    int $cacheHits,
    int $cacheMisses,
    int $sqlSelectCount,
  ): void {
    $this->assertEquals($cacheHits, $model->stats()->getCacheHits());
    $this->assertEquals($cacheMisses, $model->stats()->getCacheMisses());
    $this->assertEquals($sqlSelectCount, $model->stats()->getSqlSelects());
  }

  public function testInventory_GetById(): void {

    $inventory = new InventoryModel();

    // --
    // Read a simple item off the db.
    // --
    $id = 12387451;
    $name = 'this-is-a-test-valueset-1';

    // As a cleanup step, we need to purge LMC from reading this item again.
    $this->removeCachedItem($id);

    // This trip should hit the database.
    $oneType = $inventory->getById(ItemType::class, $id);

    $this->assertInstanceOf(ItemType::class, $oneType);

    if ($oneType instanceof ItemType) {
      $this->assertEquals($id, $oneType->id->get());
      $this->assertEquals($name, $oneType->name->get());
      $this->validateModelStats($inventory, 0, 1, 1);

    } else {
      $this->fail('type returned should of been ItemType');
    }

    // Cleanup after ourselves.
    $this->removeCachedItem($id);

  }

  public function testInventory_GetAll(): void {

  }

  // // --
  // // Lock data, and update it.
  // // --
  // $oneType->lock(); // @TODO - will successful lock will re-read the data?
  // $oneType->bar->set(1234);
  // $oneType->save();

  // // --
  // // Example: Fetching one or more rows based upon condition.
  // // Attempt to fetch where bar = 'baz'
  // // --
  // $where = new WhereClause(ItemType::class);
  // $where->and('bar', WhereOperand::EQUALS, 'baz');

  // $itemTypes = $inventory->get(ItemType::class, $where);

}
