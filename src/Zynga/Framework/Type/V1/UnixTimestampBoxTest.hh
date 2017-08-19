<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\UnixTimestampBox;
use Zynga\Framework\Type\V1\Test\TestCase;

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

  public function generateValidValues(): Vector<mixed> {
    $values = Vector {};
    $values[] = time();
    $values[] = '19770701-063000';
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = Vector {};
    $values[] = '<img';
    $values[] = 'this is not a date';
    return $values;
  }

}
