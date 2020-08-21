<?hh //strict

namespace Zynga\Framework\Cache\V2\Config\Mock\InMemory\PgDataLocks;

use
  Zynga\Framework\Cache\V2\Config\Mock\InMemory\PgDataLocks\Staging as ConfigUnderTest
;
use Zynga\Framework\Cache\V2\Exceptions\InvalidObjectForKeyCreationException;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidExampleObjectRequiredFields
;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidExampleObject
;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class StagingTest extends TestCase {

  public function createConfigUnderTest(): ConfigUnderTest {
    return new ConfigUnderTest();
  }

  public function testGetServerPairingsCorrect(): void {
    $config = $this->createConfigUnderTest();
    $servers = $config->getServerPairings();
    $this->assertEquals(0, $servers->keys()->count());
    $this->assertEquals('InMemory', $config->getDriver());
    $this->assertEquals(30, $config->getTTL());
  }

  public function testCreateKeyFromStorableObjectThrowsException(): void {
    $obj = new ValidExampleObjectRequiredFields();
    $config = $this->createConfigUnderTest();
    $this->expectException(InvalidObjectForKeyCreationException::class);
    $key = $config->createKeyFromStorableObject($obj);
  }

}
