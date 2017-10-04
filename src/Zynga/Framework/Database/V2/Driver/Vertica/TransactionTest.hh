<?hh //strict

namespace Zynga\Framework\Database\V2\Driver\Vertica;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Driver\Vertica;
use Zynga\Framework\Database\V2\Interfaces\ResultSetInterface;
use Zynga\Framework\Factory\V2\Test\MockState as FactoryMockState;

class BadVertica_Mock extends Vertica {

  public function query(string $sql): ResultSetInterface {
    throw new Exception('The world has ended');
  }

  public function nativeQuoteString(string $value): string {
    throw new Exception('The quoting');
  }

}

class TransactionTest extends TestCase {

  public function doSetUpBeforeClass(): bool {
    parent::doSetUpBeforeClass();
    DatabaseFactory::disableMockDrivers();
    DatabaseFactory::clear();
    return true;
  }

  public function testNormal(): void {

    $driver = DatabaseFactory::factory(DriverInterface::class, 'PHT_Loader');

    $transaction = $driver->getTransaction();

    $this->assertTrue($transaction->begin());
    $this->assertTrue($transaction->commit());
    $this->assertTrue($transaction->rollback());

    DatabaseFactory::clear();
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function testBadVerticaBegin(): void {
    $driver = DatabaseFactory::factory(DriverInterface::class, 'PHT_Loader');
    $mock = new BadVertica_Mock($driver->getConfig());
    $mock->getTransaction()->begin();
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function testBadVerticaCommit(): void {
    $driver = DatabaseFactory::factory(DriverInterface::class, 'PHT_Loader');
    $mock = new BadVertica_Mock($driver->getConfig());
    $mock->getTransaction()->commit();
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function testBadVerticaRollback(): void {
    $driver = DatabaseFactory::factory(DriverInterface::class, 'PHT_Loader');
    $mock = new BadVertica_Mock($driver->getConfig());
    $mock->getTransaction()->rollback();
  }

}
