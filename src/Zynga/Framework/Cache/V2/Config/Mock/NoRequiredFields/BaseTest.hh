<?hh //strict

namespace Zynga\Framework\Cache\V2\Config\Mock\NoRequiredFields;

use Zynga\Framework\Cache\V2\Config\Mock\BaseTest as MockBaseTest;
use
  Zynga\Framework\Cache\V2\Config\Mock\NoRequiredFields\Dev as ConfigUnderTest
;
use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidStorableObject;

abstract class BaseTest extends MockBaseTest {

  public function createConfigUnderTest(): DriverConfigInterface {
    return new ConfigUnderTest();
  }

  // Neither of these default tests will work for this config as it's 100% permissive of 'all' storables.
  public function testCreateKeyFromStorableObject_NotTheRightStorable(): void {
    $this->assertTrue(true); // noop test
  }

  public function testCreateKeyFromStorableObject_DefaultDataProvided(): void {
    $this->assertTrue(true); // noop test
  }

  // have to override the default here as we checksum -all- the fields
  public function testCreateKeyFromStorableObject_valid(): void {

    $obj = new ValidStorableObject();
    $obj->example_string->set('monkey');
    $obj->example_float->set(12.0);

    $config = $this->createConfigUnderTest();
    $key = $config->createKeyFromStorableObject($obj);

    $this->assertEquals(
      'zfc:f652449a796e4010ea291230869a9976:4ddb8271cced2a0a60c0888173c9d5d9',
      $key,
    );

  }

}
