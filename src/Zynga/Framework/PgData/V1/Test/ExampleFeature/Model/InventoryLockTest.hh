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
use
  Zynga\Framework\Lockable\Cache\V1\Exceptions\UnableToEstablishLockException
;

class InventoryLockTest extends BaseInventoryTest {
  
  public function testLockInGetByPk(): void {
    $inventory = new InventoryModel();
  
    // --
    // Read a simple item off the db.
    // --
    $id = 12387451;
    $name = 'this-is-a-test-valueset-1';
  
    // As a cleanup step, we need to purge LMC from reading this item again.
    $this->removeCachedItem($id);
  
    // This trip should hit the database and take the lock
    $firstTrip = $inventory->getByPk(ItemType::class, $id, true);
    if ($firstTrip instanceof ItemType) {
  
      $this->assertEquals($id, $firstTrip->id->get());
      $this->assertEquals($name, $firstTrip->name->get());
      $this->validateModelStats($inventory, 0, 1, 1);
      
      // Should already be locked now
      $this->assertTrue($inventory->cache()->getDataCache()->isLocked($firstTrip));
      
    } else {
      $this->fail('type returned should of been ItemType');
    }
  
    // unlock your obj
    if ($firstTrip instanceof ItemType) {
      $this->assertTrue($inventory->unlockRowCache($firstTrip));
      
      // Should not be locked now
      $this->assertFalse($inventory->cache()->getDataCache()->isLocked($firstTrip));
    }
    
    $this->removeCachedItem($id);
  }
  
  
  public function testLockInPgModel(): void {
    $inventory = new InventoryModel();

    // --
    // Read a simple item off the db.
    // --
    $id = 12387451;
    $name = 'this-is-a-test-valueset-1';
    $item = new ItemType($inventory);
    $item->id->set($id);
    $item->name->set($name);
    
    $this->assertTrue($inventory->lockRowCache($item));
    
    sleep(10);
    // Lock will work again as we dont solve single threaded locks. But if you run this test in two threads, one will fail.
    $this->assertTrue($inventory->lockRowCache($item));
    
    $this->assertTrue($inventory->unlockRowCache($item));
  }


}
