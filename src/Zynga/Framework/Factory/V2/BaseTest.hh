<?hh //strict

namespace Zynga\Framework\Factory\V2;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Environment\DevelopmentMode\V1\DevelopmentMode;

use Zynga\Framework\Factory\V2\Test\Factory as TestFactory;
use Zynga\Framework\Factory\V2\Template as FactoryTemplate;
use Zynga\Framework\Factory\V2\Test\Interfaces\DriverInterface as TestDriverInterface;
use Zynga\Framework\Factory\V2\Test\Interfaces\ConfigInterface as TestDriverConfigInterface;
use Zynga\Framework\Factory\V2\Exceptions\FailedToLoadDriverException;

class BaseTest extends TestCase {

  public function doSetUpBeforeClass(): bool {
    parent::doSetUpBeforeClass();
    TestFactory::clear();
    return true;
  }

  public function doTearDownAfterClass(): bool {
    parent::doTearDownAfterClass();
    TestFactory::clear();
    return true;
  }

  public function test_construct(): void {
    $foo = new FactoryTemplate("Zynga\Factory\Test");
    $this->assertTrue($foo instanceof FactoryTemplate);
    $this->assertTrue($foo->addClassRoot("Zynga\Poker\Factory\Test"));
  }

  public function test_startupState(): void {

    // There should be no drivers loaded.
    $this->assertTrue(TestFactory::clear());
    $this->assertEquals(0, TestFactory::getDriverCount());

    // Load a singular mock driver.
    $driver = TestFactory::factory(TestDriverInterface::class, 'Mock');
    $this->assertTrue($driver instanceof TestDriverInterface);
    $this->assertEquals(1, TestFactory::getDriverCount());

    // Ask for the driver again, should be cached at this point.
    $driverAgain = TestFactory::factory(TestDriverInterface::class, 'Mock');
    $this->assertTrue($driverAgain instanceof TestDriverInterface);
    $this->assertEquals(1, TestFactory::getDriverCount());

    // Clear out the driver
    $this->assertTrue(TestFactory::clear());
    $this->assertEquals(0, TestFactory::getDriverCount());

  }

  public function test_invalidConfigName(): void {
    $this->expectException(FailedToLoadDriverException::class);
    TestFactory::factory(TestDriverInterface::class, 'Invalid-Config-Name');
  }

  public function test_invalidDriverConfig(): void {
    $this->expectException(FailedToLoadDriverException::class);
    TestFactory::factory(
      TestDriverInterface::class,
      'Mock_InvalidDriverConfig',
    );
  }

  public function test_configExceptionTrap(): void {
    $this->expectException(FailedToLoadDriverException::class);
    TestFactory::factory(TestDriverInterface::class, 'Mock_ConfigThrowsException');
  }

  public function test_mockDriverTriggers(): void {
    $this->assertTrue(TestFactory::enableMockDrivers());
    $this->assertTrue(TestFactory::getUseMockDrivers());
    $this->assertTrue(TestFactory::disableMockDrivers());
    $this->assertFalse(TestFactory::getUseMockDrivers());
  }

  public function test_loadEnvironmentalConfigs(): void {

    $currentMode = DevelopmentMode::getMode();

    $devModes = array();
    $devModes[] = DevelopmentMode::DEV;
    $devModes[] = DevelopmentMode::STAGING;
    $devModes[] = DevelopmentMode::PRODUCTION;

    foreach ( $devModes as $devMode ) {

      DevelopmentMode::setMode($devMode);

      $driver = TestFactory::factory(TestDriverInterface::class, 'Mock');

      $this->assertTrue($driver instanceof TestDriverInterface);

      $config = $driver->getConfig();

      $this->assertTrue($config instanceof TestDriverConfigInterface);

      $expected = 'This-is-' . DevelopmentMode::getModeAsString();

      if ( $config instanceof TestDriverConfigInterface ) {
        $this->assertEquals($expected, $config->getExampleConfigValue());
      } else {
        $this->fail('config should be TestDriverConfigInterface');
      }

      $this->assertTrue(TestFactory::clear());

      DevelopmentMode::setMode($currentMode);

    }

    DevelopmentMode::setMode($currentMode);

  }

}
