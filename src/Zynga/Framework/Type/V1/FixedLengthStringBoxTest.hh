<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\BoolBox;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Exceptions\StringTooLongException;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Type\V1\StringBoxTest;

class FixedLengthStringBoxTest extends StringBoxTest {
  public function getTypeObject(): TypeInterface {
    return new FixedLengthStringBox();
  }

  public function testCreatingObjectSetsMaxLength(): void {
    $length = 10;
    $stringBox = new FixedLengthStringBox($length);
    $this->assertEquals($length, $stringBox->getMaxLength());
  }

  public function testCreatingDefaultObjectHasNoLengthLimit(): void {
    $stringBox = new FixedLengthStringBox();
    $this->assertEquals(
      FixedLengthStringBox::NO_LIMIT,
      $stringBox->getMaxLength(),
    );
  }

  public function testSetStringExceedingMaxLengthThrowsException(): void {
    $data = $this->getExceededMaxLengthParams();
    foreach ( $data as $testString => $testStringLength ) {
      try {
        $stringBox = new FixedLengthStringBox($testStringLength);
        $stringBox->set($testString);
      } catch ( StringTooLongException $e ) {
        $this->assertTrue(true);
      } catch ( Exception $e ) {
        $this->fail('Did not handle string being too long string=' . $testString . ' expLen=' . $testStringLength);
      }
    }
  }

  public function getExceededMaxLengthParams(): Map<string, int> {
    $data = Map {};
    $data->set('foo bar', 0);
    $data->set('foo bar baz', 3);
    return $data;
  }

  public function testSetStringUnderMaxLengthSucceeds(): void {
    $data = $this->getValidStrings();
    foreach ( $data as $testString => $testStringLength ) {
      $stringBox = new FixedLengthStringBox($testStringLength);
      $this->assertTrue($stringBox->set($testString));
    }
  }

  public function getValidStrings(): Map<string, int> {
    $data = Map {};
    $data->set('foo bar thingy', FixedLengthStringBox::NO_LIMIT);
    $data->set('foo bar baz', -1234);
    $data->set('foo bar', 100);
    return $data;
  }

}
