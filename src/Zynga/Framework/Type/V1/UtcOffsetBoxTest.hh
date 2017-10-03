<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Test\Ranged\TestCase;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\UtcOffsetBox;
use Zynga\Framework\Type\V1\Test\ValidValue;

class UtcOffsetBoxTest extends TestCase {

  public function setUp(): void {
    parent::setUp();
    $this->setMin(-11);
    $this->setMax(12);
  }

  public function getTypeObject(): TypeInterface {
    return new UtcOffsetBox();
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = parent::generateValidValues();
    return $values;
  }

}
