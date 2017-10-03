<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\FloatBox;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\Test\TestCase;
use Zynga\Framework\Type\V1\Test\ValidValue;

class FloatBoxTest extends TestCase {
  public function getTypeObject(): TypeInterface {
    return new FloatBox();
  }

  public function generateValidValues(): Vector<ValidValue> {
    $values = Vector {};

    for ($v = 0; $v < 10; $v++) {

      // generate a signed value.
      $sign = '';
      if (mt_rand(0, 1) == 1) {
        $sign = '-';
      }

      // Test a postitive float value being imported
      $floatVal = floatval($sign.mt_rand(1, 100).'.'.mt_rand(1, 1000));
      $values->add(new ValidValue($floatVal, $floatVal));

      // Test a possibly negative float value being imported
      $floatPosNegVal = floatval($sign.'.'.mt_rand(1, 1000));
      $values->add(new ValidValue($floatPosNegVal, $floatPosNegVal));

      // Test a string rep of a float value being imported.
      $whole = mt_rand(1, 100);
      $mantissa = mt_rand(1,1000);
      $floatStringVal = $sign . $whole . '.' . $mantissa;
      $values->add(new ValidValue(
        $floatStringVal,
        floatval($floatStringVal)
      ));

      // test a non-leading value ex: -.1
      $nonLeaderStringVal = $sign.'.'.mt_rand(1, 1000);
      $values->add(new ValidValue(
        $nonLeaderStringVal,
        floatval($nonLeaderStringVal)
      ));

    }

    $values->add(new ValidValue(true, 1.0));
    $values->add(new ValidValue(false, 0.0));
    $values->add(new ValidValue(1, 1.0));

    // random positive integer
    $randIntVal = mt_rand(1, 1000);
    $values->add(new ValidValue($randIntVal, floatval($randIntVal)));

    $zeroFloatVal = floatval(0);
    $values->add(new ValidValue($zeroFloatVal, $zeroFloatVal));

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
