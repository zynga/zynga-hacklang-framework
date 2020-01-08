<?hh //strict

namespace Zynga\Framework\Type\V1\Percentage\Uncapped;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Percentage\Uncapped\IntBox as IntBox;
use Zynga\Framework\Type\V1\Test\Ranged\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class IntBoxTest extends TestCase {

  public function setUp(): void {
    parent::setUp();
    $this->setMin(0);
    $this->setMax(4294967295);
  }

  public function getTypeObject(): TypeInterface {
    return new IntBox();
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = Vector {
      new ValidValue(0, 0),
      new ValidValue(100, 100),
      new ValidValue(99, 99),
      new ValidValue(4294967295, 4294967295),
    };
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = parent::generateInvalidValues();
    $values = Vector {-1};
    return $values;
  }
}
