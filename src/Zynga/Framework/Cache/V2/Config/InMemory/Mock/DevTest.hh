<?hh //strict

namespace Zynga\Framework\Cache\V2\Config\InMemory\Mock;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Cache\V2\Config\InMemory\Mock\Dev as ConfigUnderTest;

use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidExampleObject
;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidExampleObjectRequiredFields
;

class DevTest extends TestCase {

  public function createConfigUnderTest(): ConfigUnderTest {
    return new ConfigUnderTest();
  }

  public function testGetStorableObjectName(): void {

    $config = $this->createConfigUnderTest();
    $this->assertEquals(
      ValidExampleObject::class,
      $config->getStorableObjectName(),
    );

  }

  public function testServerPairings(): void {
    $config = $this->createConfigUnderTest();
    $this->assertNotNull($config->getServerPairings());
  }

  public function testTTL(): void {
    $config = $this->createConfigUnderTest();
    $this->assertEquals(3600, $config->getTTL());
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function testCreateKeyFromStorableObject_NotTheRightStorable(): void {

    $obj = new ValidExampleObjectRequiredFields();

    $config = $this->createConfigUnderTest();
    $key = $config->createKeyFromStorableObject($obj);

  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function testCreateKeyFromStorableObject_DefaultDataProvided(): void {

    $obj = new ValidExampleObject();

    $config = $this->createConfigUnderTest();
    $key = $config->createKeyFromStorableObject($obj);

  }

  public function testCreateKeyFromStorableObject_valid(): void {

    $obj = new ValidExampleObject();
    $obj->example_uint64->set(1234);

    $config = $this->createConfigUnderTest();
    $key = $config->createKeyFromStorableObject($obj);
    $this->assertEquals('lmc-ve-1234', $key);

  }

}
