<?hh //strict

namespace Zynga\Framework\Cache\V2\Config\ValidExample;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Cache\V2\Config\Mock\Production as ConfigUnderTest;

use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidExampleObject
;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidExampleObjectRequiredFields
;

class ProductionTest extends TestCase {

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
