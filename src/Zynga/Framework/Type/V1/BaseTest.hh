<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Test\TestBox;
use Zynga\Framework\Testing\TestCase\V2\Base as ZyngaTestCase;
use Zynga\Framework\Type\V1\Test\Mock\ValueGeneratorMock;
use Zynga\Framework\Type\V1\Test\Mock\BadBox;
use Zynga\Framework\Type\V1\Test\ValidValue;

use \Exception as RawException;

use \PHPUnit_Framework_AssertionFailedError;

/**
 * This class tests some functions in BaseTest
 * BaseTest is abstract, so TestBox extends it but relies on the base functions
 * Specifically __toString and jsonSerialize
 */
class BaseTest extends ZyngaTestCase {

  public function testConstruct(): void {
    $obj = new TestBox();
    //TestBox defaults to null
    $this->assertSame(null, $obj->get());
    $obj = new TestBox(1337);
    $this->assertSame(1337, $obj->get());
  }

  public function testResetRemovesValue(): void {
    $obj = new TestBox();
    //TestBox defaults to null
    $this->assertSame(null, $obj->get());
    $this->assertEquals(tuple(true, Vector {}), $obj->isDefaultValue());
    $obj->set(1337);
    $this->assertSame(1337, $obj->get());
    $this->assertEquals(tuple(false, Vector {}), $obj->isDefaultValue());
    $obj->reset();
    $this->assertSame(null, $obj->get());
    $this->assertEquals(tuple(true, Vector {}), $obj->isDefaultValue());
  }

  public function testToStringAndJsonWorksForBoolean(): void {
    $obj = new TestBox();
    $obj->set(true);
    $this->assertSame('true', json_encode($obj));
    $this->assertEquals('true', $obj);
  }

  public function testToStringAndJsonWorksForString(): void {
    $obj = new TestBox();
    $obj->set('helloWorld');
    $this->assertSame('"helloWorld"', json_encode($obj));
    $this->assertEquals('"helloWorld"', $obj);
  }

  public function testToStringAndJsonWorksForInt(): void {
    $obj = new TestBox();
    $obj->set(1337);
    $this->assertSame('1337', json_encode($obj));
    $this->assertEquals('1337', $obj);
  }

  public function testToStringAndJsonWorksForFloat(): void {
    $obj = new TestBox();
    $obj->set(13.37);
    $this->assertSame('13.37', json_encode($obj));
    $this->assertEquals('13.37', $obj);
  }

  public function testToStringAndJsonWorksForNull(): void {
    $obj = new TestBox();
    //TestBox defaults to null
    $this->assertSame('null', json_encode($obj));
    $this->assertEquals('null', $obj);
  }

  public function testGenerateValidValuesFailure_NoneProvided(): void {
    $obj = new ValueGeneratorMock('testValidValues');
    $this->expectException(PHPUnit_Framework_AssertionFailedError::class);
    $obj->testValidValues();
  }

  public function testGenerateValidInvaluesFailure_NoneProvided(): void {
    $obj = new ValueGeneratorMock('testInvalidValues');
    $this->expectException(PHPUnit_Framework_AssertionFailedError::class);
    $obj->testInvalidValues();
  }

  public function testBadBox_ValidValue_Catch(): void {
    $obj = new ValueGeneratorMock('testInvalidValues');
    $obj->setGenerateValidValues(
      Vector {new ValidValue('not-a-real-value', true)},
    );
    $this->expectException(PHPUnit_Framework_AssertionFailedError::class);
    $obj->testValidValues();
  }

  public function testBadBox_InvalidValue_Catch_Set(): void {
    $obj = new ValueGeneratorMock('testInvalidValues');
    $this->expectException(PHPUnit_Framework_AssertionFailedError::class);
    $obj->doTestInvalidValues_Against_Set(Vector {'not-a-real-value'});
  }

  public function testBadBox_InvalidValue_Catch_Strval(): void {
    $obj = new ValueGeneratorMock('testInvalidValues');
    $this->expectException(PHPUnit_Framework_AssertionFailedError::class);
    $obj->doTestInvalidValues_Against_Strval(Vector {'not-a-real-value'});
  }

  public function testBadBox_InvalidValue_SetDefaultValuesCatch(): void {
    $obj = new ValueGeneratorMock('testInvalidValues');
    $this->expectException(PHPUnit_Framework_AssertionFailedError::class);
    $obj->doTestInvalidValues_Against_SetDefaultValue(
      Vector {'not-a-real-value'},
    );
  }

  public function testValueGenerator(): void {
    $mock = new ValueGeneratorMock('testValueGenerator');
    $mock->setGenerateInvalidValues(Vector {});
    $this->assertFalse($mock->hideLogs());
  }

  public function testBadBox(): void {
    $obj = new BadBox();
    $this->assertTrue($obj->reset());
    $this->assertTrue($obj->set('this is a string'));
    $this->assertTrue($obj->set(1234));
    $this->assertTrue($obj->set(1.234));
    $this->assertTrue($obj->set(true));
  }

}
