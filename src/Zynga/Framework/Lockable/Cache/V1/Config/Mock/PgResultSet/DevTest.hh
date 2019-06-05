<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1\Config\Mock\PgResultSet;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use
  Zynga\Framework\Lockable\Cache\V1\Config\PgResultSetTest\Dev as ConfigUnderTest
;

use
  Zynga\Framework\Cache\V2\Interfaces\DriverInterface as CacheDriverInterface
;

class DevTest extends TestCase {

  public function testCreateKeyFromStorableObject(): void {

    $obj = new ConfigUnderTest();

    $this->assertInstanceOf(CacheDriverInterface::class, $obj->getCache());
    $this->assertEquals('Caching', $obj->getDriver());
    $this->assertEquals(10, $obj->getLockTTL());

  }

}
