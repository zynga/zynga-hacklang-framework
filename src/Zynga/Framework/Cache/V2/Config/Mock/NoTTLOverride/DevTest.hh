<?hh //strict

namespace Zynga\Framework\Cache\V2\Config\Mock\NoTTLOverride;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use
  Zynga\Framework\Cache\V2\Config\Mock\NoTTLOverride\Dev as ConfigUnderTest
;
use Zynga\Framework\Cache\V2\Driver\InMemory as InMemoryDriver;
use
  Zynga\Framework\Cache\V2\Exceptions\CacheDoesNotSupportTTLOverrideException
;
use Zynga\Framework\Cache\V2\Exceptions\InvalidObjectForKeyCreationException;
use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidSingleRequired as ValidSingleRequiredStorableObject
;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidExampleObjectRequiredFields
;

class DevTest extends TestCase {

  public function createConfigUnderTest(): ConfigUnderTest {
    return new ConfigUnderTest();
  }

  public function testCreateKeyFromStorableObject_NotTheRightStorable(): void {

    $obj = new ValidExampleObjectRequiredFields();

    $config = $this->createConfigUnderTest();

    $this->assertFalse($config->cacheAllowsKeyOverride());
    $this->assertFalse($config->cacheAllowsNonExpiringKeys());
    $this->assertFalse($config->cacheAllowsTTLOverride());

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

  public function testCacheAllowsTTLOverride_Fail(): void {
    CacheFactory::disableMockDrivers();
    $cache =
      CacheFactory::factory(InMemoryDriver::class, 'Mock_NoCacheKeyOverride');
    $this->expectException(CacheDoesNotSupportTTLOverrideException::class);
    $cache->getTTLSupportingOverride(1234874);
    CacheFactory::enableMockDrivers();
  }

}
