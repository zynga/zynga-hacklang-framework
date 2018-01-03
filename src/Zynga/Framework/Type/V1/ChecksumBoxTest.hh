<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\ChecksumBox;
use Zynga\Framework\Type\V1\Test\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class ChecksumBoxTest extends TestCase {

  public function getTypeObject(): TypeInterface {
    return new ChecksumBox();
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = Vector {};
    $values->add(
      new ValidValue(
        '53c246c38703ca02e1c3f67af025cde7',
        '53c246c38703ca02e1c3f67af025cde7',
      ),
    );
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = Vector {};
    $values[] = '<img';
    return $values;
  }

}
