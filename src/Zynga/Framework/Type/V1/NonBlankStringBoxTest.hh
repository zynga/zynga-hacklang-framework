<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Test\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class NonBlankStringBoxTest extends TestCase {

  <<__Override>>
  public function getTypeObject(): TypeInterface {
    return new NonBlankStringBox();
  }

  <<__Override>>
  public function generateValidValues(): Vector<ValidValue> {
    $values = Vector {};
    $values->add(new ValidValue('test', 'test'));
    $values->add(
      new ValidValue('test with white space', 'test with white space'),
    );
    $values->add(
      new ValidValue(' space padding test ', ' space padding test '),
    );
    $values->add(new ValidValue('0', '0'));

    return $values;
  }

  <<__Override>>
  public function generateInvalidValues(): Vector<mixed> {
    $values = Vector {};
    $values[] = '';
    $values[] = ' ';
    $values[] = '    ';
    $values[] = "\n";
    $values[] = "\t";
    $values[] = "\r\n";

    return $values;
  }

}
