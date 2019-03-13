<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\UnixTimestampBox;
use Zynga\Framework\Type\V1\Test\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class UnixTimestampBoxTest extends TestCase {

  public function getTypeObject(): TypeInterface {
    return new UnixTimestampBox();
  }

  public function testNativeValue(): void {

    $time = time();

    $obj = $this->getTypeObject();
    $obj->set($time);

    if ($obj instanceof UnixTimestampBox) {
      $this->assertEquals($time, $obj->get());
    }
  }

  public function generateValidValues(): Vector<ValidValue> {

    $values = Vector {};

    $currentTime = time();
    $values->add(new ValidValue($currentTime, $currentTime));
    $values->add(new ValidValue('19770701-063000', 623846092208790));

    return $values;

  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = Vector {};
    $values[] = '<img';
    $values[] = 'this is not a date';
    return $values;
  }

  public function testTimeDifferenceInDaysReturnsTheCorrectDifference(): void {
    $initialTime = 10 * UnixTimestampBox::SECONDS_IN_DAY;
    $testingBox = new UnixTimestampBox($initialTime);

    $testCases = Vector {
      new ValidValue($initialTime, 0),
      new ValidValue(9 * UnixTimestampBox::SECONDS_IN_DAY, -1),
      new ValidValue(11 * UnixTimestampBox::SECONDS_IN_DAY, 1),
      new ValidValue(14.5 * UnixTimestampBox::SECONDS_IN_DAY, 4),
      new ValidValue(6.5 * UnixTimestampBox::SECONDS_IN_DAY, -3),
    };

    foreach ($testCases as $case) {
      $endTime = intval($case->getInputValue());
      $timeDifference = $testingBox->timeDifferenceInDays(new UnixTimestampBox($endTime));
      $this->assertEquals($case->getExpectedValue(), $timeDifference);
    }
  }
}
