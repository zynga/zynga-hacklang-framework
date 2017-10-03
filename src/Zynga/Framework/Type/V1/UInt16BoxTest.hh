<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Test\Ranged\TestCase;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\UInt16Box;
use Zynga\Framework\Type\V1\Test\ValidValue;

class UInt16BoxTest extends TestCase {

  public function setUp(): void {
    parent::setUp();
    $this->setMin(0);
    $this->setMax(65535);
  }

  public function getTypeObject(): TypeInterface {
    return new UInt16Box();
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = parent::generateValidValues();
    $values->add(new ValidValue(true, intval(true)));
    $values->add(new ValidValue(false, intval(false)));
    $values->add(new ValidValue(3.0, intval(3.0)));
    $values->add(new ValidValue(6.0, intval(6.0)));
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = parent::generateInvalidValues();
    $values[] = 3.5;
    $values[] = 4.00123;
    return $values;
  }
}
