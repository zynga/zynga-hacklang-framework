<?hh //strict

namespace Zynga\Framework\Database\V2\Driver;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Driver\Mock as MockDriver;
use Zynga\Framework\Database\V2\Interfaces\ResultSetInterface;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;
use Zynga\Framework\Factory\V2\Test\MockState as FactoryMockState;
use
  Zynga\Framework\Environment\ErrorCapture\V1\Interfaces\ErrorCaptureInterface
;

class MockTest extends TestCase {
  const string TEST_DRIVER = 'Mock';

  public function doSetUpBeforeClass(): bool {

    parent::doSetUpBeforeClass();

    DatabaseFactory::enableMockDrivers();
    DatabaseFactory::clear();

    return true;

  }

  public function testValid(): void {

    $mock =
      DatabaseFactory::factory(DriverInterface::class, self::TEST_DRIVER);

    $this->assertTrue($mock->connect());
    $this->assertTrue($mock->disconnect());
    $this->assertTrue($mock->setIsConnected(false));
    $this->assertFalse($mock->getIsConnected());
    $this->assertEquals('NOOP', $mock->getLastError());
    $this->assertFalse($mock->hadError());
    $this->assertEquals('booga', $mock->nativeQuoteString('booga'));
    $this->assertClassImplements(QuoteInterface::class, $mock->quote());
    $this->assertClassImplements(
      TransactionInterface::class,
      $mock->transaction(),
    );

    if ($mock instanceof Mock) {

      $this->assertTrue($mock->resetResultSets());
      $this->assertTrue($mock->addEmptyResultSet());

      $resultSet = $mock->query('SELECT * FROM DUAL');

      $this->assertTrue($resultSet instanceof ResultSetInterface);

      $this->assertClassImplements(
        ErrorCaptureInterface::class,
        $resultSet->errorCapture(),
      );
      $this->assertTrue($mock->addResultSet(Vector {1, 2, 3}));

    }

  }

  /**
   * @expectedException Zynga\Framework\Database\V2\Exceptions\QueryFailedException
   */
  public function testBrokenQuery(): void {
    $mock =
      DatabaseFactory::factory(DriverInterface::class, self::TEST_DRIVER);

    if ($mock instanceof Mock) {
      $this->assertTrue($mock->resetResultSets());
      $this->assertTrue($mock->addFailingResultSet());
      $mock->query('SELECT * FROM DUAL');
    }

  }

  public function testLoadResultsForTestEmpty(): void {
    $mock =
      DatabaseFactory::factory(DriverInterface::class, self::TEST_DRIVER);
    if ($mock instanceof Mock) {
      $this->assertTrue($mock->loadResultsForTest(self::class, __FUNCTION__));
    }
  }

  /**
   * @expectedException Zynga\Framework\Database\V2\Exceptions\Mock\LoadTestDataNoFileFoundException
   */
  public function testLoadResultsForTestNoFileFound(): void {
    $mock =
      DatabaseFactory::factory(DriverInterface::class, self::TEST_DRIVER);
    if ($mock instanceof Mock) {
      $this->assertTrue($mock->loadResultsForTest(self::class, __FUNCTION__));
    }
  }

  public function testLoadResultsWithData(): void {
    $mock =
      DatabaseFactory::factory(DriverInterface::class, self::TEST_DRIVER);
    if ($mock instanceof Mock) {
      $this->assertTrue($mock->loadResultsForTest(self::class, __FUNCTION__));
    }
  }

  /**
   * @expectedException Zynga\Framework\Database\V2\Exceptions\Mock\BadResultOffsetException
   */
  public function testGetResultSetsPastSet(): void {
    $mock =
      DatabaseFactory::factory(DriverInterface::class, self::TEST_DRIVER);
    if ($mock instanceof Mock) {
      $mock->getResultSet('no-context', 99);
    }
  }

  public function testLoadResultsWithVector(): void {
    $mock =
      DatabaseFactory::factory(DriverInterface::class, self::TEST_DRIVER);
    if ($mock instanceof Mock) {
      $this->assertTrue($mock->loadResultsForTest(self::class, __FUNCTION__));
    }
  }

  public function testLoadResultsBadData(): void {
    $mock =
      DatabaseFactory::factory(DriverInterface::class, self::TEST_DRIVER);
    if ($mock instanceof Mock) {
      $this->assertTrue($mock->loadResultsForTest(self::class, __FUNCTION__));
    }
  }

}
