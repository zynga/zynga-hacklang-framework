<?hh //strict

namespace Zynga\Framework\Factory\V2;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Factory\V2\DriverContainer;

use Zynga\Framework\Factory\V2\Test\Config\Mock\Dev as TestConfig;
use Zynga\Framework\Factory\V2\Test\Driver\Mock as TestDriver;

class DriverContainerTest extends TestCase {

  public function createTestContainer(): DriverContainer<TestDriver> {
    $container = new DriverContainer();
    return $container;
  }

  public function test_construct(): void {
    $obj = $this->createTestContainer();
    $this->assertTrue($obj instanceof DriverContainer);
  }

  public function test_isDriverCached(): void {
    $obj = $this->createTestContainer();

    $driverName = 'TestDriver';

    // no driver, yet
    $this->assertFalse($obj->isDriverCached($driverName));

    // add driver to cache
    $config = new TestConfig();
    $driver = new TestDriver($config);

    $this->assertTrue($obj->addDriverToCache($driverName, $driver));
    $this->assertTrue($obj->isDriverCached($driverName));

    // remove driver from cache
    $this->assertTrue($obj->removeDriverFromCache($driverName));
    $this->assertFalse($obj->isDriverCached($driverName));

  }

  public function test_getDriverFromCache(): void {

    $obj = $this->createTestContainer();

    $driverName = 'TestDriver';

    // add driver to cache
    $config = new TestConfig();
    $driver = new TestDriver($config);

    $this->assertTrue($obj->addDriverToCache($driverName, $driver));
    $this->assertTrue($obj->isDriverCached($driverName));

    $driver2 = $obj->getDriverFromCache($driverName);
    $this->assertTrue($driver2 instanceof TestDriver);

  }

  <<
  expectedException(
    "Zynga\Framework\Factory\V2\Exceptions\DriverContainer\DriverNotInCacheException",
  )
  >>
  public function test_getDriverFromCache_Failure(): void {
    $obj = $this->createTestContainer();

    $driverName = 'TestDriver';
    $obj->getDriverFromCache($driverName);
  }

  public function test_addDriverToCache(): void {
    $obj = $this->createTestContainer();

    $driverName = 'TestDriver';

    // add driver to cache
    $config = new TestConfig();
    $driver = new TestDriver($config);

    $this->assertTrue($obj->addDriverToCache($driverName, $driver));

  }

  <<
  expectedException(
    "Zynga\Framework\Factory\V2\Exceptions\DriverContainer\DriverAlreadyInCacheException",
  )
  >>
  public function test_addDriverToCache_Failure(): void {
    $obj = $this->createTestContainer();

    $driverName = 'TestDriver';

    // add driver to cache
    $config = new TestConfig();
    $driver = new TestDriver($config);

    $obj->addDriverToCache($driverName, $driver);
    $obj->addDriverToCache($driverName, $driver);

  }

  public function test_removeDriverFromCache(): void {
    $obj = $this->createTestContainer();

    $driverName = 'TestDriver';

    // add driver to cache
    $config = new TestConfig();
    $driver = new TestDriver($config);

    $this->assertTrue($obj->addDriverToCache($driverName, $driver));
    $this->assertEquals(1, $obj->count());

    $this->assertTrue($obj->removeDriverFromCache($driverName));
    $this->assertEquals(0, $obj->count());

  }

  <<
  expectedException(
    "Zynga\Framework\Factory\V2\Exceptions\DriverContainer\DriverNotInCacheException",
  )
  >>
  public function test_removeDriverFromCache_Failure(): void {
    $obj = $this->createTestContainer();

    $driverName = 'TestDriver';

    $obj->removeDriverFromCache($driverName);

  }

  public function test_clear(): void {
    $obj = $this->createTestContainer();

    $driverName = 'TestDriver';

    // add driver to cache
    $config = new TestConfig();
    $driver = new TestDriver($config);

    $this->assertTrue($obj->addDriverToCache($driverName, $driver));
    $this->assertEquals(1, $obj->count());

    $this->assertTrue($obj->clear());
    $this->assertEquals(0, $obj->count());

  }

}
