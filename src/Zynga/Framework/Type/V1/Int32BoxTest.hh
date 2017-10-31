<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Int32Box;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Test\Ranged\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class Int32BoxTest extends TestCase {

  public function setUp(): void {
    parent::setUp();
    $this->setMin(-2147483648);
    $this->setMax(2147483647);
  }

  public function getTypeObject(): TypeInterface {
    return new Int32Box();
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = parent::generateValidValues();
    $values->add(new ValidValue(true, intval(true)));
    $values->add(new ValidValue(false, intval(false)));
    $values->add(new ValidValue(2983.0, intval(2983.0)));
    $values->add(new ValidValue(8974524.0, intval(8974524.0)));
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = parent::generateInvalidValues();
    $values[] = 3.5;
    $values[] = 4.00123;
    $values[] = -3.5;
    $values[] = -4.00123;
    return $values;
  }
}
