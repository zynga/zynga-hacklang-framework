<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Base as BaseBox;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;

class FloatBox extends BaseBox {
  private float $_value;

  public function __construct() {
    parent::__construct();
    $this->_value = 0.0;
  }

  public function reset(): bool {
    $this->importFromFloat(0.0);
    $this->setIsDefaultValue(true);
    return true;
  }

  public function get(): float {
    return $this->_value;
  }

  <<__Override>>
  protected function importFromBool(bool $value): bool {
    $floatValue = 0.0;
    if ($value === true) {
      $floatValue = 1.0;
    }

    return $this->importFromFloat($floatValue);
  }

  <<__Override>>
  protected function importFromFloat(float $value): bool {
    $this->_value = $value;
    return true;
  }

  <<__Override>>
  protected function importFromInteger(int $value): bool {
    return $this->importFromFloat(floatval($value));
  }

  <<__Override>>
  protected function importFromString(string $value): bool {

    $floatRegex = '';
    $floatRegex .= '[+-]?('; // optional leading sign
    $floatRegex .= '([0-9]+e?[+-]?[0-9]*)'; // integer mantissa, integer exponent
    $floatRegex .= '|('; // OR, it's got a real mantissa
    $floatRegex .= '(';
    $floatRegex .= '([0-9]+\.[0-9]*)'; // real mantissa (with digits to the left of the decimal point)
    $floatRegex .= '|(\.[0-9]+)'; // fraction-only mantissa
    $floatRegex .= ')';
    $floatRegex .= '(e[+-]?[0-9]+)?'; // integer exponent for real mantissas
    $floatRegex .= ')'; // (PHP does not allow real exponents in the xxxEyyy format.)
    $floatRegex .= ')';

    if (preg_match('/^'.$floatRegex.'$/i', $value)) {
      return $this->importFromFloat(floatval($value));
    }

    throw new FailedToImportFromStringException('value='.$value);
  }

}
