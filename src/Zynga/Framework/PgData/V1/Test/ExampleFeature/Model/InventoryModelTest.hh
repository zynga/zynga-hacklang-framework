<?hh // strict

namespace Zynga\Framework\PgData\V1\Test\ExampleFeature\Model;

use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Lockable\Cache\V1\Factory as LockableCacheFactory;

use Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\InventoryModel;
use Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\Inventory\ItemType;

class InventoryModelTest extends TestCase {

  public function doSetUpBeforeClass(): bool {

    parent::doSetUpBeforeClass();

    CacheFactory::disableMockDrivers();
    DatabaseFactory::disableMockDrivers();
    LockableCacheFactory::disableMockDrivers();

    return true;

  }

  // public function doTearDownAfterClass(): bool {
  //   CacheFactory::enableMockDrivers();
  //   DatabaseFactory::enableMockDrivers();
  //   return true;
  // }

  public function testInventory_GetById(): void {

    $inventory = new InventoryModel();

    // --
    // Read a simple item off the db.
    // --
    $id = 12387451;
    $name = 'this-is-a-test-valueset-1';

    $oneType = $inventory->getById(ItemType::class, $id);

    $this->assertInstanceOf(ItemType::class, $oneType);

    if ($oneType instanceof ItemType) {
      $this->assertEquals($id, $oneType->id->get());
      $this->assertEquals($name, $oneType->name->get());
    } else {
      $this->fail('type returned should of been ItemType');
    }

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
