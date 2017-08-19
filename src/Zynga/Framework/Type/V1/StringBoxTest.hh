<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\Test\TestCase;

class StringBoxTest extends TestCase {

  public function getTypeObject(): TypeInterface {
    return new StringBox();
  }

  public function testNativeValue(): void {
    $obj = $this->getTypeObject();
    $obj->set('some-value');

    if ($obj instanceof StringBox) {
      $this->assertEquals('some-value', $obj->get());
    }
  }

  public function generateValidValues(): Vector<mixed> {
    $values = Vector {};
    $values[] = '0';
    $values[] = '1';
    $values[] = 'true';
    $values[] = 'false';
    $values[] = '';
    $values[] = true;
    $values[] = false;
    $values[] = 0.0;
    $values[] = 1.0;
    $values[] = 0;
    $values[] = 1;
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

    foreach ( $allTestableData as $testData ) {
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

}

class StringAndLengthData {
  public string $testString;
  public int $testLength;
  public function __construct(string $testString, int $testLength) {
    $this->testString = $testString;
    $this->testLength = $testLength;
  }
}
