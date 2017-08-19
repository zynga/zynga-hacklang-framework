<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Test\Ranged\TestCase;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\UInt16Box;

class UInt16BoxTest extends TestCase {

  public function setUp(): void {
    parent::setUp();
    $this->setMin(0);
    $this->setMax(65535);
  }

  public function getTypeObject(): TypeInterface {
    return new UInt16Box();
  }

  public function generateValidValues(): Vector<mixed> {
    $values = parent::generateValidValues();
    $values[] = true;
    $values[] = false;
    $values[] = 3.0;
    $values[] = 6.0;
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = parent::generateInvalidValues();
    $values[] = 3.5;
    $values[] = 4.00123;
    return $values;
  }
}
