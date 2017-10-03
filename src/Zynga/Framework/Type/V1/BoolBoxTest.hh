<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\BoolBox;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Test\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class BoolBoxTest extends TestCase {
  public function getTypeObject(): TypeInterface {
    return new BoolBox();
  }

  public function testNativeValue(): void {
    $obj = $this->getTypeObject();
    $obj->set(true);
    if ($obj instanceof BoolBox) {
      $this->assertTrue($obj->get());
    }
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = Vector {};
    $values->add(new ValidValue('0', false));
    $values->add(new ValidValue('1', true));
    $values->add(new ValidValue('true', true));
    $values->add(new ValidValue('false', false));
    $values->add(new ValidValue(true, true));
    $values->add(new ValidValue(false, false));
    $values->add(new ValidValue(0.0, false));
    $values->add(new ValidValue(1.0, true));
    $values->add(new ValidValue(0, false));
    $values->add(new ValidValue(1, true));
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = Vector {};
    $values[] = 'sorta true';
    $values[] = 'sorta not true';
    $values[] = 'null';
    $values[] = 1.05;
    $values[] = 0.5;
    $values[] = -1;
    $values[] = '';
    return $values;
  }
}
