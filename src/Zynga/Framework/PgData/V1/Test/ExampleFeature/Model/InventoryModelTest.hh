<?hh // strict

namespace Zynga\Framework\PgData\V1\Test\ExampleFeature\Model;

use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Lockable\Cache\V1\Factory as LockableCacheFactory;
use
  Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface as LockableCacheDriverInterface
;
use Zynga\Framework\PgData\V1\Interfaces\PgWhereClauseInterface;
use Zynga\Framework\PgData\V1\PgModel;
use Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\InventoryModel;
use Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\Inventory\ItemType;
use Zynga\Framework\PgData\V1\PgWhereClause;
use Zynga\Framework\PgData\V1\PgWhereOperand;

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

    $inv = new InventoryModel();
    $obj = new ItemType($inv);
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

    // Run the same get again, it should be cached.
    $twoType = $inventory->getById(ItemType::class, $id);

    if ($twoType instanceof ItemType) {
      $this->assertEquals($id, $twoType->id->get());
      $this->assertEquals($name, $twoType->name->get());
      $this->validateModelStats($inventory, 1, 1, 1);

    } else {
      $this->fail('type returned should of been ItemType');
    }

    // Cleanup after ourselves.
    $this->removeCachedItem($id);

  }

  public function testInventory_GetById_NoRows(): void {

    $inventory = new InventoryModel();

    // --
    // Read a simple item off the db.
    // --
    $id = 0;

    // As a cleanup step, we need to purge LMC from reading this item again.
    $this->removeCachedItem($id);

    // This trip should hit the database.
    $oneType = $inventory->getById(ItemType::class, $id);

    $this->assertEquals(null, $oneType);
    $this->validateModelStats($inventory, 0, 1, 1);

    // Run the same get again, it should be not be cached.
    $twoType = $inventory->getById(ItemType::class, $id);

    $this->assertEquals(null, $twoType);
    $this->validateModelStats($inventory, 0, 2, 2);

    // Cleanup after ourselves.
    $this->removeCachedItem($id);

  }

  private function doesQueryReturnExpectedValues(
    Vector<Map<string, mixed>> $expectedResultToInclude,
    ?PgWhereClauseInterface $where = null,
  ): void {

    // TODO: replace with factory.
    $inventory = new InventoryModel();

    $resultSet = $inventory->get(ItemType::class, $where);

    foreach ($expectedResultToInclude as $expectedResult) {

      $foundExpected = false;
      foreach ($resultSet->items() as $resultObj) {
        if (!$resultObj instanceof ItemType) {
          continue;
        }
        if ($resultObj->id->get() == $expectedResult['id'] &&
            $resultObj->name->get() == $expectedResult['name']) {
          $foundExpected = true;
          $this->assertEquals($expectedResult['id'], $resultObj->id->get());
          $this->assertEquals(
            $expectedResult['name'],
            $resultObj->name->get(),
          );
          break;
        }
      }

      if ($foundExpected == false) {
        $this->fail('Failed to find '.json_encode($expectedResult));
      }

    }

  }

  public function testInventory_EmptySet(): void {

    $model = new InventoryModel();

    $where = new PgWhereClause($model);
    $where->and('id', PgWhereOperand::EQUALS, 0);

    $result = $model->get(ItemType::class, $where);

    $this->assertEquals(0, $result->count());

  }

  public function testInventory_GetAll(): void {

    // --
    // JEO: This is a kind of special test in that we have 'known' data that has to
    // be within the set, however if other people are adding or removing values within
    // the table at the same time, we don't want a sporadic failure.
    // --
    // TODO: Remove result set cache.

    // This is a get-all example, get all rows on this table.
    $where = null; // normally you use: new WhereClause() and add clauses.

    $expectedResultToInclude = Vector {};
    $expectedResultToInclude->add(
      Map {'id' => 12387451, 'name' => 'this-is-a-test-valueset-1'},
    );
    $expectedResultToInclude->add(
      Map {'id' => 12387452, 'name' => 'this-is-a-test-valueset-2'},
    );
    $expectedResultToInclude->add(
      Map {'id' => 12387453, 'name' => 'this-is-a-test-valueset-3'},
    );
    $expectedResultToInclude->add(
      Map {'id' => 12387454, 'name' => 'this-is-a-test-valueset-4'},
    );
    $expectedResultToInclude->add(
      Map {'id' => 12387455, 'name' => 'this-is-a-test-valueset-5'},
    );

    $this->doesQueryReturnExpectedValues($expectedResultToInclude, $where);

    // TODO: Remove result set cache.

  }

  public function testInventory_EqualsTest(): void {

    $model = new InventoryModel();

    $where = new PgWhereClause($model);
    $where->and('name', PgWhereOperand::EQUALS, 'this-is-a-test-valueset-3');

    $expectedResultToInclude = Vector {};

    $expectedResultToInclude->add(
      Map {'id' => 12387453, 'name' => 'this-is-a-test-valueset-3'},
    );

    $this->doesQueryReturnExpectedValues($expectedResultToInclude, $where);

  }

  public function testInventory_GreaterThanTest(): void {

    $model = new InventoryModel();

    $where = new PgWhereClause($model);
    $where->and('id', PgWhereOperand::GREATER_THAN, 12387452);
    $where->and('id', PgWhereOperand::LESS_THAN, 12387455);

    $expectedResultToInclude = Vector {};

    $expectedResultToInclude->add(
      Map {'id' => 12387453, 'name' => 'this-is-a-test-valueset-3'},
    );

    $expectedResultToInclude->add(
      Map {'id' => 12387454, 'name' => 'this-is-a-test-valueset-4'},
    );

    $this->doesQueryReturnExpectedValues($expectedResultToInclude, $where);

  }

  public function testInventory_IdIncrementing(): void {

    // stand up the model
    $model = new InventoryModel();

    // stand up the row you'd like to add
    $item = new ItemType($model);

    $firstId = $item->getPrimaryKeyNextValue();

    $this->assertGreaterThan(0, $firstId->get());

    $secondId = $item->getPrimaryKeyNextValue();

    $this->assertGreaterThan($firstId->get(), $secondId->get());

  }

  public function testInventory_Add(): void {

    // generate a test name to run against
    $testName = 'this-is-a-phpunit-test-'.time().'-'.mt_rand(200);

    // stand up the model
    $model = new InventoryModel();

    // stand up the row you'd like to add
    $item = new ItemType($model);

    // Test for the pk to be at it's default state.
    list($isDefault, $defaultError) = $item->id->isDefaultValue();
    $this->assertTrue($isDefault);

    $item->name->set($testName);

    $this->assertTrue($model->add($item));

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
