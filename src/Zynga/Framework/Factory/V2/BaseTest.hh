<?hh //strict

namespace Zynga\Framework\Factory\V2;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Environment\DevelopmentMode\V1\DevelopmentMode;

use Zynga\Framework\Factory\V2\Test\Factory as TestFactory;
use Zynga\Framework\Factory\V2\Template as FactoryTemplate;
use
  Zynga\Framework\Factory\V2\Test\Interfaces\DriverInterface as TestDriverInterface
;
use
  Zynga\Framework\Factory\V2\Test\Interfaces\ConfigInterface as TestDriverConfigInterface
;
use Zynga\Framework\Factory\V2\Exceptions\FailedToLoadDriverException;
use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Factory\V2\Test\Mock\TemplateReturningWrongDriver;
use
  Zynga\Framework\Factory\V2\Test\Config\Mock\InvalidDriverConfig\Dev as MockDriverConfigDev
;
use
  Zynga\Framework\Factory\V2\Test\Config\Mock\InvalidDriverConfig\Staging as MockDriverConfigStg
;
use
  Zynga\Framework\Factory\V2\Test\Config\Mock\InvalidDriverConfig\Production as MockDriverConfigProd
;

class BaseTest extends TestCase {

  public function doSetUpBeforeClass(): bool {
    parent::doSetUpBeforeClass();
    TestFactory::clearFactoryTemplates();
    return true;
  }

  public function doTearDownAfterClass(): bool {
    parent::doTearDownAfterClass();
    TestFactory::clearFactoryTemplates();
    return true;
  }

  public function tearDown(): void {
    TestFactory::clearOverridenMockDrivers();
    parent::tearDown();
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
    TestFactory::factory(
      TestDriverInterface::class,
      'Mock_ConfigThrowsException',
    );
  }

  public function test_mockDriverTriggers(): void {
    $this->assertTrue(TestFactory::enableMockDrivers());
    $this->assertTrue(TestFactory::getUseMockDrivers());
    $this->assertTrue(TestFactory::disableMockDrivers());
    $this->assertFalse(TestFactory::getUseMockDrivers());
  }

  public function test_loadEnvironmentalConfigs(): void {
    $driverConfigs = Map {
      "Dev" => new MockDriverConfigDev(),
      "Staging" => new MockDriverConfigStg(),
      "Production" => new MockDriverConfigProd(),
    };
  }

  public function testAddClassRoot(): void {
    $this->assertTrue(TestFactory::addClassRoot('\\SomeOther\ClassRoot'));
  }

  public function testOverridingMockDrivers(): void {
    TestFactory::enableMockDrivers();
    TestFactory::clearOverridenMockDrivers();

    TestFactory::overrideMockDriver('Mock', 'MockOverride');
    $driver = TestFactory::factory(TestDriverInterface::class, 'Mock');
    $config = $driver->getConfig();

    $expected = 'This-is-overridden-'.DevelopmentMode::getModeAsString();
    if ($config instanceof TestDriverConfigInterface) {
      $this->assertEquals($expected, $config->getExampleConfigValue());
    } else {
      $this->fail('config should be TestDriverConfigInterface');
    }
  }

  public function testMockDriversClassPathsAreNotOverriden(): void {
    TestFactory::enableMockDrivers();
    TestFactory::clearOverridenMockDrivers();

    $driver = TestFactory::factory(TestDriverInterface::class, 'Mock\Reader');
    $driver2 =
      TestFactory::factory(TestDriverInterface::class, 'Mock_Reader');

    $expected = 'This-is-'.DevelopmentMode::getModeAsString().'-Reader';

    $config = $driver->getConfig();
    $config2 = $driver2->getConfig();

    if ($config instanceof TestDriverConfigInterface &&
        $config2 instanceof TestDriverConfigInterface) {
      $this->assertEquals($expected, $config->getExampleConfigValue());
      $this->assertEquals($expected, $config2->getExampleConfigValue());
    } else {
      $this->fail('config should be TestDriverConfigInterface');
    }
  }

  public function testFactoryLoadWithWrongInterface(): void {

    // --
    // Descructive test here basically overloading the internal factory template object in
    // order to get a type violation that should never happen in the real world.
    // --
    TestFactory::clear();
    DatabaseFactory::clear();

    TestFactory::setFactoryTemplate(
      TestFactory::getClassRoot(),
      new TemplateReturningWrongDriver(DatabaseFactory::getClassRoot()),
    );

    $this->expectException(FailedToLoadDriverException::class);
    TestFactory::factory(TestDriverInterface::class, 'Test_Mysql');

    // clear the bad template.
    TestFactory::clearFactoryTemplates();

  }

  public function testFactoryClearTemplates(): void {
    $this->assertTrue(TestFactory::clearFactoryTemplates());
  }

}
