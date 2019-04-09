<?hh //strict

namespace Zynga\Framework\Type\V1\Percentage\Capped;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Percentage\Capped\IntBox as IntBox;
use Zynga\Framework\Type\V1\Test\Ranged\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class IntBoxTest extends TestCase {

  public function setUp(): void {
    parent::setUp();
    $this->setMin(0);
    $this->setMax(100);
  }

  public function getTypeObject(): TypeInterface {
    return new IntBox();
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = Vector {
      new ValidValue(0, 0),
      new ValidValue(100, 100),
      new ValidValue(99, 99),
    };
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = parent::generateInvalidValues();
    $values = Vector {-1, 101};
    return $values;
  }
}
