<?hh //strict

namespace Zynga\Framework\Type\V1\Percentage\Uncapped;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use
  Zynga\Framework\Type\V1\Percentage\Uncapped\FloatingPointBox as PercentageBox
;
use Zynga\Framework\Type\V1\Test\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class FloatingPointBoxTest extends TestCase {

  public function getTypeObject(): TypeInterface {
    return new PercentageBox();
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = Vector {
      new ValidValue(0.0, 0.0),
      new ValidValue(100.0, 100.0),
      new ValidValue(99.5, 99.5),
      new ValidValue(1073741824.0, 1073741824.0),
    };
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = Vector {-1.0};
    return $values;
  }
}
