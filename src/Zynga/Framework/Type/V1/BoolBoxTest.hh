<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\BoolBox;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Test\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;
use Zynga\Framework\Type\V1\ChecksumBox;

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

  public function testValuesNotEqual(): void {
    $true = new BoolBox();
    $true->set(true);
    $false = new BoolBox();
    $false->set(false);
    $this->assertFalse($true->equals($false));
  }

  public function testNullNotEqual(): void {
    $true = new BoolBox();
    $true->set(true);
    $this->assertFalse($true->equals(null));
  }

  public function testWrongClassNotEqual(): void {
    $true = new BoolBox();
    $true->set(true);
    $checksum = new ChecksumBox();
    $checksum->set(1);
    $this->assertFalse($true->equals($checksum));
  }

  public function testValuesEqual(): void {
    $true = new BoolBox();
    $true->set(true);
    $true2 = new BoolBox();
    $true2->set(true);
    $this->assertTrue($true->equals($true2));
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
