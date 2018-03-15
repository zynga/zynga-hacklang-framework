<?hh //strict

namespace Zynga\Framework\Database\V2\Config\Mock;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Config\Mock\Staging as ConfigUnderTest;

use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;

class StagingTest extends TestCase {

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

    // run the full init.
    $config->init();

    $this->assertEquals(
      ConfigUnderTest::SERVER_USERNAME,
      $config->getUserName(),
    );
    $this->assertEquals(
      ConfigUnderTest::SERVER_PASSWORD,
      $config->getPassword(),
    );

    $pregs = array();

    // echo 'connStr=' . $config->getConnectionString() . "\n";

    if (preg_match(
          '/^username\=(.*)\;password\=(.*)\;host\=(.*)\;$/',
          $config->getConnectionString(),
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
      'Zynga\Framework\Database\V2\Interfaces\QuoteInterface',
      $driver->getQuoter(),
    );
    $this->assertClassImplements(
      'Zynga\Framework\Database\V2\Interfaces\TransactionInterface',
      $driver->getTransaction(),
    );
  }

}
