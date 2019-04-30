<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Mock;

use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\Factory\V2\Test\MockState as FactoryMockState;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Poker\Type\Snid\V1\BoxFactory as SnidBoxFactory;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;

class TransactionTest extends TestCase {

  public function doSetUpBeforeClass(): bool {

    parent::doSetUpBeforeClass();

    DatabaseFactory::enableMockDrivers();
    DatabaseFactory::clear();

    return true;

  }

  public function testNormal(): void {

    $driver = DatabaseFactory::factory(DriverInterface::class, 'Mock');
    $driver->setSnUid(SnidBoxFactory::facebook(), new UidBox(1));

    $transaction = $driver->getTransaction();

    $this->assertTrue($transaction->begin());
    $this->assertTrue($transaction->commit());
    $this->assertTrue($transaction->rollback());

  }

}
