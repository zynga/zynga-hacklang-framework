<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver;

use Zynga\Framework\Database\V2\Exceptions\Mock\BadResultOffsetException;
use Zynga\Framework\Database\V2\Exceptions\QueryFailedException;
use Zynga\Framework\Database\V2\Exceptions\Mock\LoadTestDataNoFileFoundException;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\ShardedDatabase\V3\Driver\Mock;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\ResultSetInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;

use Zynga\Framework\Factory\V2\Test\MockState as FactoryMockState;

class MockTest extends TestCase {

  public function doSetUpBeforeClass(): bool {

    parent::doSetUpBeforeClass();

    DatabaseFactory::enableMockDrivers();
    DatabaseFactory::clear();

    return true;

  }

  public function testValid(): void {

    $mock = DatabaseFactory::factory(DriverInterface::class, 'Read');

    $this->assertTrue($mock->connect());
    $this->assertTrue($mock->disconnect());
    $this->assertTrue($mock->setIsConnected(false));
    $this->assertFalse($mock->getIsConnected());
    $this->assertEquals('NOOP', $mock->getLastError());
    $this->assertFalse($mock->hadError());
    $this->assertEquals('booga', $mock->nativeQuoteString('booga'));

    if ($mock instanceof Mock) {
      $this->assertTrue($mock->resetResultsSets());
      $this->assertTrue($mock->addEmptyResultSet());
      $this->assertTrue($mock->connectToShard(0, ""));
      $this->assertTrue(
        $mock->query('SELECT * FROM DUAL') instanceof ResultSetInterface,
      );

      $this->assertTrue($mock->addResultSet(Vector {1,2,3}));

    }

  }

  public function testBrokenQuery(): void {
    $mock = DatabaseFactory::factory(DriverInterface::class, 'Read');

    if ($mock instanceof Mock) {
      $this->assertTrue($mock->resetResultsSets());
      $this->assertTrue($mock->addFailingResultSet());
      $this->expectException(QueryFailedException::class);
      $mock->query('SELECT * FROM DUAL');
    }

  }

  public function testLoadResultsForTestEmpty(): void {
    $mock = DatabaseFactory::factory(DriverInterface::class, 'Read');
    if ($mock instanceof Mock) {
      $this->assertTrue($mock->loadResultsForTest(self::class, __FUNCTION__));
    }
  }

  public function testLoadResultsForTestNoFileFound(): void {
    $mock = DatabaseFactory::factory(DriverInterface::class, 'Mock');
    if ($mock instanceof Mock) {
      $this->expectException(LoadTestDataNoFileFoundException::class);
      $this->assertTrue($mock->loadResultsForTest(self::class, __FUNCTION__));
    }
  }

  public function testLoadResultsWithData(): void {
    $mock = DatabaseFactory::factory(DriverInterface::class, 'Mock');
    if ($mock instanceof Mock) {
      $this->assertTrue($mock->loadResultsForTest(self::class, __FUNCTION__));
    }
  }

  public function testGetResultSetsPastSet(): void {
    $mock = DatabaseFactory::factory(DriverInterface::class, 'Mock');
    if ($mock instanceof Mock) {
      $this->expectException(BadResultOffsetException::class);
      $mock->getResultSet('no-context', 99);
    }
  }

}
