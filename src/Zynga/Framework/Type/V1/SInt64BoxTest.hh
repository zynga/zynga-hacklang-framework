<?hh //strict

namespace Zynga\Type\V1;

use Zynga\Framework\Type\V1\Test\Ranged\TestCase;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\SInt64Box;

class SInt64BoxTest extends TestCase {

  public function setUp(): void {
    parent::setUp();
    $this->setMin(PHP_INT_MIN);
    $this->setMax(PHP_INT_MAX);
  }

  public function getTypeObject(): TypeInterface {
    return new SInt64Box();
  }

  public function generateValidValues(): Vector<mixed> {
    $values = parent::generateValidValues();
    $values[] = true;
    $values[] = false;
    $values[] = 2983.0;
    $values[] = 8974524.0;
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
