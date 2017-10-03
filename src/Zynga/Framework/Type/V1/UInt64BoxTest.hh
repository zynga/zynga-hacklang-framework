<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Test\Ranged\TestCase;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\UInt64Box;
use Zynga\Framework\Type\V1\Test\ValidValue;

class UInt64BoxTest extends TestCase {

  public function setUp(): void {
    parent::setUp();
    $this->setMin(0);
    $this->setMax(PHP_INT_MAX);
  }

  public function getTypeObject(): TypeInterface {
    return new UInt64Box();
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = parent::generateValidValues();
    $values->add(new ValidValue(true, intval(true)));
    $values->add(new ValidValue(false, intval(false)));
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = parent::generateInvalidValues();
    $values->add(12.2);
    return $values;
  }

  public function test_constructorPass(): void {
    $obj = new UInt64Box(123);
    $this->assertEquals($obj->get(), 123);
  }
  
}
