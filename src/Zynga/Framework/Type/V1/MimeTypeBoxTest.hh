<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\MimeTypeBox;
use Zynga\Framework\Type\V1\Test\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class MimeTypeBoxTest extends TestCase {

  public function getTypeObject(): TypeInterface {
    return new MimeTypeBox();
  }

  public function testNativeValue(): void {
    $obj = $this->getTypeObject();
    $obj->set(MimeTypeBox::JSON);
    if ($obj instanceof MimeTypeBox) {
      $this->assertEquals(MimeTypeBox::JSON, $obj->get());
    }
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = Vector {};
    $values->add(new ValidValue(MimeTypeBox::JSON, MimeTypeBox::JSON));
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = Vector {};
    $values[] = '<img';
    return $values;
  }
}
