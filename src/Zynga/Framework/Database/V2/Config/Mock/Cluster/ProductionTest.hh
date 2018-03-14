<?hh //strict

namespace Zynga\Framework\Database\V2\Config\Mock\Cluster;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use
  Zynga\Framework\Database\V2\Config\Mock\Cluster\Production as ConfigUnderTest
;

use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;

class ProductionTest extends TestCase {

  /**
   * Test the constants for our config
   */
  public function testConstants(): void {
    $this->assertEquals(1, ConfigUnderTest::SERVER_ID_START);
    $this->assertEquals(5, ConfigUnderTest::SERVER_ID_STOP);
    $this->assertEquals('testusername', ConfigUnderTest::TEST_USERNAME);
    $this->assertEquals('testpassword', ConfigUnderTest::TEST_PASSWORD);
  }

  /**
   * Test the values are actually getting set via the config init
   */
  public function testValidInit(): void {

    $config = new ConfigUnderTest();

    // run the full init.
    $config->init();

    $this->assertEquals(
      ConfigUnderTest::TEST_USERNAME,
      $config->getUserName(),
    );
    $this->assertEquals(
      ConfigUnderTest::TEST_PASSWORD,
      $config->getPassword(),
    );

    $pregs = array();

    // echo 'connStr=' . $config->getConnectionString() . "\n";

    if (preg_match(
          '/^username\=(.*)\;password\=(.*)\;host\=(.*)\;$/',
          $config->getConnectionString(),
          &$pregs,
        )) {
      $this->assertEquals(ConfigUnderTest::TEST_USERNAME, $pregs[1]);
      $this->assertEquals(ConfigUnderTest::TEST_PASSWORD, $pregs[2]);
    }

    $this->assertEquals('Mock', $config->getDriver());

    // At this point we should be able to pull a native driver off
    $driver =
      DatabaseFactory::factory(DriverInterface::class, 'Mock_Cluster');
    $this->assertClassImplements(DriverInterface::class, $driver);
    $this->assertTrue($driver->connect());
    $this->assertFalse($driver->hadError());
    $this->assertEquals('NOOP', $driver->getLastError());

    // Given a native driver we should be able to pull a quoter
    $quoter = $driver->getQuoter();
    $this->assertClassImplements(
      'Zynga\Framework\Database\V2\Interfaces\QuoteInterface',
      $quoter,
    );

    // Given a native driver we should be able to pull a transaction
    $transaction = $driver->getTransaction();
    $this->assertClassImplements(
      'Zynga\Framework\Database\V2\Interfaces\TransactionInterface',
      $transaction,
    );
    $this->assertFalse($config->isDatabaseReadOnly());
    $this->assertEquals(ConfigUnderTest::TEST_SCHEMA, $config->getSchema());
  }

}
