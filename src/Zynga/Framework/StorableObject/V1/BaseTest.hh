<?hh //strict

namespace Zynga\Framework\StorableObject\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\StorableObject\V1\Test\Mock\Valid;
use Zynga\Framework\StorableObject\V1\Test\Mock\ProtobufValid;

use Zynga\Framework\StorableObject\V1\Test\Mock\InvalidUnsupportedType;
use Zynga\Framework\StorableObject\V1\Test\Mock\ValidNested;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\StorableObject\V1\Exceptions\ExpectedFieldCountMismatchException;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Environment\CodePath\V1\CodePath;

use Zynga\Framework\Type\V1\StringBox;

class BaseTest extends TestCase {

  public function testValid(): void {

    // test init state
    $obj = new Valid();

    $this->assertEquals('', $obj->example_string->get());

    $this->assertEquals(0, $obj->example_uint64->get());
    $this->assertEquals(0.0, $obj->example_float->get());

    $testStringValue = 'string_value_65';
    $testIntValue = 89012;
    $testFloatValue = 12938.123;

    $testMap = Map {};
    $testMap['example_string'] = $testStringValue;
    $testMap['example_uint64'] = $testIntValue;
    $testMap['example_float'] = $testFloatValue;

    $obj->import()->fromMap($testMap);

    $this->assertEquals($testStringValue, $obj->example_string->get());
    $this->assertEquals($testIntValue, $obj->example_uint64->get());
    $this->assertEquals($testFloatValue, $obj->example_float->get());

  }

  public function testIsRequired(): void {
    $obj = new Valid();
    $this->assertFalse($obj->getIsRequired());
    $this->assertTrue($obj->setIsRequired(true));
    $this->assertTrue($obj->getIsRequired());
    $this->assertTrue($obj->setIsRequired(false));
    $this->assertFalse($obj->getIsRequired());
  }

  public function testIsDefaultValue(): void {
    $obj = new Valid();
    list($isDefaultValue, $defaultValues) = $obj->isDefaultValue();
    $this->assertTrue($isDefaultValue);
    $this->assertTrue($obj->setIsDefaultValue(false));
    list($isDefaultValue, $defaultValues) = $obj->isDefaultValue();
    $this->assertFalse($isDefaultValue);
  }

}
