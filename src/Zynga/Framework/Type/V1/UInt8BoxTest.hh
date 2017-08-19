<?hh //strict

namespace Zynga\Type\V1;

use Zynga\Framework\Type\V1\Test\Ranged\TestCase;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\UInt8Box;
use Zynga\Framework\Type\V1\Test\ValidValue;

class UInt8BoxTest extends TestCase {

  public function setUp(): void {
    parent::setUp();
    $this->setMin(0);
    $this->setMax(255);
  }

  public function getTypeObject(): TypeInterface {
    return new UInt8Box();
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = parent::generateValidValues();
    $values->add(new ValidValue(true, intval(true)));
    $values->add(new ValidValue(false, intval(false)));
    $values->add(new ValidValue(4.0, intval(4.0)));
    $values->add(new ValidValue(20.0, intval(20.0)));
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = parent::generateInvalidValues();
    $values[] = 3.5;
    $values[] = 4.00123;
    return $values;
  }
}
