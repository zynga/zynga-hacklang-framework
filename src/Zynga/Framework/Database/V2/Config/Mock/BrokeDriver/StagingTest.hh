<?hh //strict

namespace Zynga\Framework\Database\V2\Config\Mock\BrokeDriver;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use
  Zynga\Framework\Database\V2\Interfaces\DriverInterface as DatabaseDriverInterface
;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;
use
  Zynga\Framework\Database\V2\Config\Mock\BrokeDriver\Staging as ConfigUnderTest
;

class StagingTest extends TestCase {

  /**
   * Test the constants for our config
   */
  public function testConstants(): void {
    $this->assertEquals(1, ConfigUnderTest::SERVER_ID_START);
    $this->assertEquals(5, ConfigUnderTest::SERVER_ID_STOP);
    $this->assertEquals('someuser', ConfigUnderTest::SERVER_USERNAME);
    $this->assertEquals('somepass', ConfigUnderTest::SERVER_PASSWORD);
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
          &$pregs,
        )) {
      $this->assertEquals(ConfigUnderTest::SERVER_USERNAME, $pregs[1]);
      $this->assertEquals(ConfigUnderTest::SERVER_PASSWORD, $pregs[2]);
    }

    $this->assertEquals('NoSuchDriver', $config->getDriver());
    $this->assertFalse($config->isDatabaseReadOnly());

    // This is special as this isn't a legit driver, so we fake it for coverage.
    $driver = DatabaseFactory::factory(
      DatabaseDriverInterface::class,
      'Mock_Cluster',
    );
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
