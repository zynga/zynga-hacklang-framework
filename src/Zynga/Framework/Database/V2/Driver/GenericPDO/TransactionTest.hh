<?hh //strict

namespace Zynga\Framework\Database\V2\Driver\GenericPDO;

use \ReflectionMethod;
use
  Zynga\Framework\Database\V2\Driver\GenericPDO\Mock\Transaction as MockTransaction
;
use Zynga\Framework\Database\V2\Driver\GenericPDO\Transaction;
use Zynga\Framework\Database\V2\Exceptions\Mock\BadResultOffsetException;
use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Factory\V2\Test\MockState as FactoryMockState;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class TransactionTest extends TestCase {

  public function doSetUpBeforeClass(): bool {
    parent::doSetUpBeforeClass();
    DatabaseFactory::enableMockDrivers();
    return true;
  }

  public function testNormalFlow(): void {
    $driver =
      DatabaseFactory::factory(DriverInterface::class, 'Mock_Cluster');
    $mockTransaction = new MockTransaction($driver);
    $this->assertTrue($mockTransaction->begin());
    $this->assertTrue($mockTransaction->commit());
    $this->assertTrue($mockTransaction->rollback());
    $transaction = new Transaction($driver);
    $method = new ReflectionMethod($transaction, 'execute');
    $method->setAccessible(true);
    $this->expectException(BadResultOffsetException::class);
    $method->invoke($transaction, 'ROLLBACK');
  }
}
