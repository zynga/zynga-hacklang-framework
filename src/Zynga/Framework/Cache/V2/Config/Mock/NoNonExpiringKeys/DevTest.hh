<?hh //strict

namespace Zynga\Framework\Cache\V2\Config\Mock\NoNonExpiringKeys;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use
  Zynga\Framework\Cache\V2\Config\Mock\NoNonExpiringKeys\Dev as ConfigUnderTest
;
use Zynga\Framework\Cache\V2\Driver\InMemory as InMemoryDriver;
use Zynga\Framework\Cache\V2\Exceptions\CacheRequiresTTLException;
use Zynga\Framework\Cache\V2\Exceptions\InvalidObjectForKeyCreationException;
use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidStorableObject;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidSingleRequired as ValidSingleRequiredStorableObject
;

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

  public function testCreateKeyFromStorableObject_NotTheRightStorable(): void {

    $config = $this->createConfigUnderTest();

    $obj = new ValidStorableObject();

    $this->expectException(InvalidObjectForKeyCreationException::class);

    $key = $config->createKeyFromStorableObject($obj);

  }

  public function testCreateKeyFromStorableObject_DefaultDataProvided(): void {

    $obj = new ValidStorableObject();

    $config = $this->createConfigUnderTest();

    $this->expectException(InvalidObjectForKeyCreationException::class);
    $key = $config->createKeyFromStorableObject($obj);

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

  public function testNoNonExpiringKeys_Fail(): void {

    CacheFactory::disableMockDrivers();

    $cache =
      CacheFactory::factory(InMemoryDriver::class, 'Mock_NoNonExpiringKeys');
    $this->expectException(CacheRequiresTTLException::class);
    $cache->getTTLSupportingOverride(0);

    CacheFactory::enableMockDrivers();

  }

}
