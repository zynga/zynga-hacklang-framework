<?hh //strict

namespace Zynga\Framework\Datadog\V2\Config;

use Zynga\Framework\Environment\DevelopmentMode\V1\DevelopmentMode;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Datadog\V2\Factory as DatadogFactory;
use
  Zynga\Framework\Datadog\V2\Interfaces\DriverInterface as DatadogDriverInterface
;

abstract class BaseTest extends TestCase {

  public function __construct(string $name) {

    parent::__construct($name);

  }

  public function doSetUpBeforeClass(): bool {
    parent::doSetUpBeforeClass();
    return true;
  }

  public function doTearDownAfterClass(): bool {
    parent::doTearDownAfterClass();
    return true;
  }

  // get the driver name that is under test.
  abstract public function getDriverName(): string;

  // get the development mode we need to be in to do our testing.
  abstract public function getDevelopmentMode(): int;

  public function testLoadConfig(): void {

    $testDevMode = $this->getDevelopmentMode();

    DevelopmentMode::setMode($testDevMode);

    DatadogFactory::disableMockDrivers();
    DatadogFactory::clear();

    $driverName = $this->getDriverName();

    $driver =
      DatadogFactory::factory(DatadogDriverInterface::class, $driverName);

    $this->assertNotEmpty($driver->getConfig()->getDriver());
    $this->assertNotEmpty($driver->getConfig()->getServerHostname());
    $this->assertNotEmpty($driver->getConfig()->getServerPort());

  }

}
