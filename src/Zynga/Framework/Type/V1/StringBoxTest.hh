<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\Test\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class StringBoxTest extends TestCase {

  public function getTypeObject(): TypeInterface {
    return new StringBox();
  }

  public function testIsStringValid(): void {
    $stringBox = new StringBox();
    $this->assertFalse($stringBox->isStringValid('script:'));
    $this->assertTrue($stringBox->isStringValid('198.161.0.0'));
  }

  public function testNativeValue(): void {
    $obj = $this->getTypeObject();
    $obj->set('some-value');

    if ($obj instanceof StringBox) {
      $this->assertEquals('some-value', $obj->get());
    }
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = Vector {};
    $values->add(new ValidValue('0', '0'));
    $values->add(new ValidValue('1', '1'));
    $values->add(new ValidValue('true', 'true'));
    $values->add(new ValidValue('false', 'false'));
    $values->add(new ValidValue('', ''));
    $values->add(new ValidValue(true, strval(true)));
    $values->add(new ValidValue(false, strval(false)));
    $values->add(new ValidValue(0.0, strval(0.0)));
    $values->add(new ValidValue(1.0, strval(1.0)));
    $values->add(new ValidValue(0, strval(0)));
    $values->add(new ValidValue(1, strval(1)));
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = Vector {};
    $values[] = '<img';
    return $values;
  }

  public function testToStringConversion(): void {
    $foo = new StringBox();
    $foo->set('someTextValue');
    $this->assertEquals('someTextValue', "$foo");
  }

  public function testExploitFlags(): void {
    $foo = new StringBox();

    // should be enabled by default
    $this->assertTrue($foo->isExploitDetectionEnabled());

    // turn off the detection
    $this->assertTrue($foo->disableExploitDetection());
    $this->assertFalse($foo->isExploitDetectionEnabled());

    // turn on the detection
    $this->assertTrue($foo->enableExploitDetection());
    $this->assertTrue($foo->isExploitDetectionEnabled());
  }

  public function testGetLengthReturnsCorrectSize(): void {

    $allTestableData = $this->getStringAndLengths();

    foreach ($allTestableData as $testData) {
      $foo = new StringBox();
      $foo->set($testData->testString);
      $this->assertEquals($testData->testLength, $foo->getLength());

    }
  }

  public function getStringAndLengths(): Vector<StringAndLengthData> {
    $vec = Vector {};
    $vec->add(new StringAndLengthData('', 0));
    $vec->add(new StringAndLengthData('1234', 4));
    $vec->add(new StringAndLengthData('ʩʩʩʩ', 4)); // Make sure that UTF8 characters are counted as single characters
    return $vec;
  }

  public function testInvalidMBData(): void {
    $foo = new StringBox();
    $foo->set("");
    $this->assertEquals(0, $foo->getLength("UTF-8"));
  }

  public function testSetToRandomEmptyCharSet(): void {
    $stringBox = new StringBox();
    $result = $stringBox->setToRandom(1, 10, '');
    $this->assertTrue($result);
    $this->assertEquals('', $stringBox->get());
  }

  public function testSetToRandomNegativeMinLength(): void {
    $stringBox = new StringBox();
    $result = $stringBox->setToRandom(-100, 1, 'abcd');
    $this->assertTrue($result);
    $this->assertEquals('', $stringBox->get());
  }

  public function testSetToRandomNegativeaxLength(): void {
    $stringBox = new StringBox();
    $result = $stringBox->setToRandom(1, -100, 'abcd');
    $this->assertTrue($result);
    $this->assertEquals('', $stringBox->get());
  }

  public function testSetToRandomMinLengthGreaterThanMax(): void {
    $stringBox = new StringBox();
    $result = $stringBox->setToRandom(100, 1, 'abcd');
    $this->assertTrue($result);
    $this->assertEquals('', $stringBox->get());
  }

  public function testSetToRandomSuccessful(): void {
    $stringBox = new StringBox();
    $result = $stringBox->setToRandom(5, 15, 'abcd');
    $this->assertTrue($result);
    $string = $stringBox->get();
    $stringLength = strlen($string);
    $this->assertTrue($stringLength <= 15);
    $this->assertTrue($stringLength >= 5);
    for($i = 0; $i < $stringLength; ++$i) {
      $this->assertTrue(strpos('abcd', $string[$i]) !== false);
    }
  }

}

class StringAndLengthData {
  public string $testString;
  public int $testLength;
  public function __construct(string $testString, int $testLength) {
    $this->testString = $testString;
    $this->testLength = $testLength;
  }
}
