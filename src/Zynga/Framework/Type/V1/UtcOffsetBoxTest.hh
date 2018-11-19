<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Test\Ranged\TestCase;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\UtcOffsetBox;
use Zynga\Framework\Type\V1\Test\ValidValue;

class UtcOffsetBoxTest extends TestCase {

  public function setUp(): void {
    parent::setUp();
    $this->setMin(-12);
    $this->setMax(14);
  }

  public function getTypeObject(): TypeInterface {
    return new UtcOffsetBox();
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = parent::generateValidValues();
    return $values;
  }

}
