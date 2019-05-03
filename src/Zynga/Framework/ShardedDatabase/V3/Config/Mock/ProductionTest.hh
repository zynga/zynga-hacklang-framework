<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Mock;

use
  Zynga\Framework\ShardedDatabase\V3\Config\Mock\Base as BaseConfig
;
use Zynga\Framework\ShardedDatabase\V3\Config\Mock\BaseTest;
use
  Zynga\Framework\ShardedDatabase\V3\Config\Mock\Production as ConfigUnderTest
;
use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\QuoteInterface;
use
  Zynga\Framework\ShardedDatabase\V3\Interfaces\TransactionInterface
;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;
use Zynga\Framework\Type\V1\UInt64Box;

class ProductionTest extends BaseTest {

  /**
   * Test the constants for our config
   */
  public function testConstants(): void {
    $this->assertEquals('mockdb', ConfigUnderTest::CURRENT_DATABASE);
    $this->assertEquals('mockhost', ConfigUnderTest::CURRENT_SERVER);
    $this->assertEquals(1337, ConfigUnderTest::SERVER_PORT);
    $this->assertEquals('mockuser', ConfigUnderTest::SERVER_USERNAME);
    $this->assertEquals('mockpass', ConfigUnderTest::SERVER_PASSWORD);
  }

  /**
   * Test the values are actually getting set via the config init
   */
  public function testValidInit(): void {

    $config = new ConfigUnderTest();

    $server =
      $config->getServerFromShardType(new UInt64Box(1));

    // run the full init.
    $config->init();

    $this->assertEquals(
      ConfigUnderTest::SERVER_USERNAME,
      $server->getUsername(),
    );
    $this->assertEquals(
      ConfigUnderTest::SERVER_PASSWORD,
      $server->getPassword(),
    );

    $this->assertEquals('localhost', $server->getHostname());
    $this->assertEquals(
      0,
      $config->getShardId(new UInt64Box(1)),
    );
    $this->assertEquals('mockhost', $config->getCurrentServer());

    $pregs = array();

    // echo 'connStr=' . $config->getConnectionString() . "\n";

    if (preg_match(
          '/^username\=(.*)\;password\=(.*)\;host\=(.*)\;$/',
          $config->getConnectionString(
            new UInt64Box(1)
          ),
          $pregs,
        )) {
      $this->assertEquals(ConfigUnderTest::SERVER_USERNAME, $pregs[1]);
      $this->assertEquals(ConfigUnderTest::SERVER_PASSWORD, $pregs[2]);
    }

    $this->assertEquals('Mock', $config->getDriver());
    $this->assertFalse($config->isDatabaseReadOnly());

    // This is special as this isn't a legit driver, so we fake it for coverage.
    $driver = DatabaseFactory::factory(DriverInterface::class, 'Mock');
    $this->assertClassImplements(
      'Zynga\Framework\ShardedDatabase\V3\Interfaces\QuoteInterface',
      $driver->getQuoter(),
    );
    $this->assertClassImplements(
      'Zynga\Framework\ShardedDatabase\V3\Interfaces\TransactionInterface',
      $driver->getTransaction(),
    );

  }

  public function getConfigUnderTest(): BaseConfig {
    return new ConfigUnderTest();
  }
}
