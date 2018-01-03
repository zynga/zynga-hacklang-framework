<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\HttpResponseCodeBox;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Test\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class HttpResponseCodeBoxTest extends TestCase {
  public function getTypeObject(): TypeInterface {
    return new HttpResponseCodeBox();
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = Vector {};

    $values->add(
      new ValidValue(
        HttpResponseCodeBox::HTTP_OK,
        HttpResponseCodeBox::HTTP_OK,
      ),
    );

    $values->add(
      new ValidValue(
        HttpResponseCodeBox::HTTP_FAILURE,
        HttpResponseCodeBox::HTTP_FAILURE,
      ),
    );

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
    $values[] = 0;
    return $values;
  }
}
