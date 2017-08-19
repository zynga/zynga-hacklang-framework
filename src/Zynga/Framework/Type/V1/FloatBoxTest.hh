<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\FloatBox;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Test\TestCase;

class FloatBoxTest extends TestCase {
  public function getTypeObject(): TypeInterface {
    return new FloatBox();
  }

  public function generateValidValues(): Vector<mixed> {
    $values = Vector {};
    for ($v = 0; $v < 10; $v++) {
      // generate a signed value.
      $sign = '';
      if (mt_rand(0, 1) == 1) {
        $sign = '-';
      }

      $values[] = floatval($sign.mt_rand(1, 100).'.'.mt_rand(1, 1000));
      $values[] = floatval($sign.'.'.mt_rand(1, 1000));
      $values[] = $sign.mt_rand(1, 100).'.'.mt_rand(1, 1000);
      $values[] = $sign.'.'.mt_rand(1, 1000);
    }

    $values[] = true;
    $values[] = false;
    $values[] = 1;
    $values[] = mt_rand(1, 1000);
    $values[] = floatval(0);
    return $values;
  }

  public function generateInvalidValues(): Vector<mixed> {
    $values = Vector {};
    $values[] = '$foo.bar';
    $values[] = 'a string';
    $values[] = '.';
    $values[] = '';
    return $values;
  }
}
