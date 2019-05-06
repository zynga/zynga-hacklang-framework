<?hh //strict

namespace Zynga\Framework\Database\V2\Driver\Vertica;

use Zynga\Framework\Database\V2\Driver\Vertica;
use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\ResultSetInterface;
use Zynga\Framework\Database\V2\Test\Driver\Vertica\BadVertica;
use Zynga\Framework\Database\V2\Test\Driver\Vertica\BadVerticaException;
use Zynga\Framework\Factory\V2\Test\MockState as FactoryMockState;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class TransactionTest extends TestCase {

  public function doSetUpBeforeClass(): bool {
    parent::doSetUpBeforeClass();
    DatabaseFactory::disableMockDrivers();
    DatabaseFactory::clear();
    return true;
  }

  public function testNormal(): void {

    $driver =
      DatabaseFactory::factory(DriverInterface::class, 'Test_Vertica');

    $transaction = $driver->getTransaction();

    $this->assertTrue($transaction->begin());
    $this->assertTrue($transaction->commit());
    $this->assertTrue($transaction->rollback());

    DatabaseFactory::clear();
  }

  public function testBadVerticaBegin(): void {

    $driver =
      DatabaseFactory::factory(DriverInterface::class, 'Test_Vertica');

    $mock = new BadVertica($driver->getConfig());

    $this->expectException(BadVerticaException::class);
    $mock->getTransaction()->begin();

  }

  public function testBadVerticaCommit(): void {

    $driver =
      DatabaseFactory::factory(DriverInterface::class, 'Test_Vertica');

    $mock = new BadVertica($driver->getConfig());

    $this->expectException(BadVerticaException::class);
    $mock->getTransaction()->commit();

  }

  public function testBadVerticaRollback(): void {

    $driver =
      DatabaseFactory::factory(DriverInterface::class, 'Test_Vertica');

    $mock = new BadVertica($driver->getConfig());

    $this->expectException(BadVerticaException::class);
    $mock->getTransaction()->rollback();

  }

}
