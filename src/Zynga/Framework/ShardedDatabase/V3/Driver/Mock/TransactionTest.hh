<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Mock;

use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\Factory\V2\Test\MockState as FactoryMockState;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\UInt64Box;

class TransactionTest extends TestCase {

  public function doSetUpBeforeClass(): bool {

    parent::doSetUpBeforeClass();

    DatabaseFactory::enableMockDrivers();
    DatabaseFactory::clear();

    return true;

  }

  public function testNormal(): void {

    $driver = DatabaseFactory::factory(DriverInterface::class, 'Mock');
    $driver->setShardType(new UInt64Box(1));

    $transaction = $driver->getTransaction();

    $this->assertTrue($transaction->begin());
    $this->assertTrue($transaction->commit());
    $this->assertTrue($transaction->rollback());

  }

}
