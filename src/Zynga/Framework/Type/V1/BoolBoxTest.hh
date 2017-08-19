<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\BoolBox;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Test\TestCase;

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

  public function generateValidValues(): Vector<mixed> {
    $values = Vector {};
    $values[] = '0';
    $values[] = '1';
    $values[] = 'true';
    $values[] = 'false';
    $values[] = true;
    $values[] = false;
    $values[] = 0.0;
    $values[] = 1.0;
    $values[] = 0;
    $values[] = 1;
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
