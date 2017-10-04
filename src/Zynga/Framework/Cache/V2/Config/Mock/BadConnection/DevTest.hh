<?hh //strict

namespace Zynga\Framework\Cache\V2\Config\Mock\BadConnection;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Cache\V2\Config\Mock\BadConnection\Dev as ConfigUnderTest;

use Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidExampleObject;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidExampleObjectRequiredFields
;

class DevTest extends TestCase {

  public function createConfigUnderTest(): ConfigUnderTest {
    return new ConfigUnderTest();
  }

  public function testGetServerPairings(): void {

    $config = $this->createConfigUnderTest();
    $servers = $config->getServerPairings();
    $this->assertEquals(1, $servers->keys()->count());
    $this->assertEquals('Memcache', $config->getDriver());
    $this->assertEquals(3600, $config->getTTL());

  }

  public function testGetStorableObjectName(): void {

    $config = $this->createConfigUnderTest();
    $this->assertEquals(
      ValidExampleObject::class,
      $config->getStorableObjectName(),
    );

  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function testCreateKeyFromStorableObject_ExceptionWired(): void {

    $obj = new ValidExampleObjectRequiredFields();
    $config = $this->createConfigUnderTest();
    $key = $config->createKeyFromStorableObject($obj);

  }

}
