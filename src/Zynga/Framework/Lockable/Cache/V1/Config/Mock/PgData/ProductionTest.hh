<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1\Config\Mock\PgData;

use
  Zynga\Framework\Cache\V2\Interfaces\DriverInterface as CacheDriverInterface
;
use
  Zynga\Framework\Lockable\Cache\V1\Config\Mock\PgData\Production as ConfigUnderTest
;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class ProductionTest extends TestCase {

  public function testCreateKeyFromStorableObject(): void {
    $obj = new ConfigUnderTest();
    $this->assertInstanceOf(CacheDriverInterface::class, $obj->getCache());
    $this->assertEquals('Caching', $obj->getDriver());
    $this->assertEquals(10, $obj->getLockTTL());
  }

}
