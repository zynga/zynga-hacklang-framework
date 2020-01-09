<?hh //strict

namespace Zynga\Framework\Datadog\V2\Config;

use Zynga\Framework\Datadog\V2\Factory as DatadogFactory;
use
  Zynga\Framework\Datadog\V2\Interfaces\DriverConfigInterface as DatadogDriverConfigInterface
;
use
  Zynga\Framework\Datadog\V2\Interfaces\DriverInterface as DatadogDriverInterface
;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

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

  abstract public function getDriverConfigToTest(
  ): DatadogDriverConfigInterface;

  public function testConfigContainsValidInfo(): void {
    DatadogFactory::disableMockDrivers();
    DatadogFactory::clear();

    $driverConfig = $this->getDriverConfigToTest();

    $this->assertNotEmpty($driverConfig->getDriver());
    $this->assertNotEmpty($driverConfig->getServerHostname());
    $this->assertNotEmpty($driverConfig->getServerPort());
  }
}
