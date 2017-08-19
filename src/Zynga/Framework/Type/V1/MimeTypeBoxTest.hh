<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\MimeTypeBox;
use Zynga\Framework\Type\V1\Test\TestCase;

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

  public function generateValidValues(): Vector<mixed> {
    $values = Vector {};
    $values[] = MimeTypeBox::JSON;
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = Vector {};
    $values[] = '<img';
    return $values;
  }
}
