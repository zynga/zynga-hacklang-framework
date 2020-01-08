<?hh //strict

namespace Zynga\Framework\Cache\V2\Config\Mock\YesTTLOverride;

use
  Zynga\Framework\Cache\V2\Config\Mock\YesTTLOverride\Dev as ConfigUnderTest
;
use Zynga\Framework\Cache\V2\Driver\InMemory as InMemoryDriver;
use Zynga\Framework\Cache\V2\Exceptions\CacheTTLExceededException;
use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidSingleRequired as ValidSingleRequiredStorableObject
;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidExampleObjectRequiredFields
;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class DevTest extends TestCase {

  public function createConfigUnderTest(): ConfigUnderTest {
    return new ConfigUnderTest();
  }

  public function testConfig_Settings(): void {

    $config = $this->createConfigUnderTest();

    $this->assertFalse($config->cacheAllowsKeyOverride());
    $this->assertFalse($config->cacheAllowsNonExpiringKeys());
    $this->assertTrue($config->cacheAllowsTTLOverride());

  }

  public function testCreateKeyFromStorableObject_valid(): void {

    $obj = new ValidSingleRequiredStorableObject();
    $obj->example_uint64->set(1234);

    $config = $this->createConfigUnderTest();
    $key = $config->createKeyFromStorableObject($obj);
    $this->assertEquals(
      'zfc:61ca750d827b16e43d493251a0cc1ba8:81dc9bdb52d04dc20036dbd8313ed055',
      $key,
    );

  }

  public function testCacheExceedsTTLOverride_Fail(): void {
    CacheFactory::disableMockDrivers();
    $cache = CacheFactory::factory(InMemoryDriver::class, 'Mock');
    $ttl = $cache->getConfig()->getTTL();
    $ttl += 1;
    $this->expectException(CacheTTLExceededException::class);
    $cache->getTTLSupportingOverride($ttl);
    CacheFactory::enableMockDrivers();
  }

}
