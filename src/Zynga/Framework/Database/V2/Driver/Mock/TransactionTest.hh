<?hh //strict

namespace Zynga\Framework\Database\V2\Driver\Mock;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Factory\V2\Test\MockState as FactoryMockState;

class TransactionTest extends TestCase {

  public function doSetUpBeforeClass(): bool {

    parent::doSetUpBeforeClass();

    DatabaseFactory::enableMockDrivers();
    DatabaseFactory::clear();

    return true;

  }

  public function testNormal(): void {

    $driver = DatabaseFactory::factory(DriverInterface::class, 'Mock_Cluster');

    $transaction = $driver->getTransaction();

    $this->assertTrue($transaction->begin());
    $this->assertTrue($transaction->commit());
    $this->assertTrue($transaction->rollback());

  }

}
