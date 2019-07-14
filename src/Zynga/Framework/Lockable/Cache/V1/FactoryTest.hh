<?hh //strict

namespace Zynga\Framework\Lockable\Cache\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Environment\DevelopmentMode\V1\DevelopmentMode;

use Zynga\Framework\Lockable\Cache\V1\Factory as CacheFactory;
use Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverConfigInterface;
use Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface;

class FactoryTest extends TestCase {

  public function testGetClassRoot(): void {
    $this->assertEquals(
      '\Zynga\Framework\Lockable\Cache\V1',
      CacheFactory::getClassRoot(),
    );
  }

  public function testValidExample(): void {
    $this->assertTrue(
      CacheFactory::factory(DriverInterface::class, 'Mock') instanceof DriverInterface,
    );
  }

  public function testClose(): void {
    CacheFactory::disableMockDrivers();
    CacheFactory::clear();
    $this->assertEquals(0, CacheFactory::getDriverCount());
    CacheFactory::factory(DriverInterface::class, 'Mock');
    $this->assertEquals(1, CacheFactory::getDriverCount());
    CacheFactory::clear();
    $this->assertEquals(0, CacheFactory::getDriverCount());
  }

  <<
  expectedException(
    "Zynga\Framework\Factory\V2\Exceptions\FailedToLoadDriverException",
  )
  >>
  public function testFactory_BadRequested(): void {
    CacheFactory::disableMockDrivers();
    CacheFactory::factory(DriverInterface::class, 'NotReal');
  }

}
