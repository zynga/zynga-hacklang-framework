<?hh //strict

namespace Zynga\Framework\Type\V1\Percentage\Capped;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use
  Zynga\Framework\Type\V1\Percentage\Capped\FloatingPointBox as PercentageBox
;
use Zynga\Framework\Type\V1\Test\Ranged\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class FloatingPointBoxTest extends TestCase {

  public function setUp(): void {
    parent::setUp();
    $this->setMin(0);
    $this->setMax(100);
  }

  public function getTypeObject(): TypeInterface {
    return new PercentageBox();
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = Vector {
      new ValidValue(0.0, 0.0),
      new ValidValue(100.0, 100.0),
      new ValidValue(99.5, 99.5),
    };
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = parent::generateInvalidValues();
    $values = Vector {-1.0, 100.01};
    return $values;
  }
}
