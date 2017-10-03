<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Base as BaseBox;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromFloatException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromIntegerException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;

class BoolBox extends BaseBox {
  private bool $_value;

  public function __construct() {
    parent::__construct();
    $this->_value = false;
  }

  public function reset(): bool {
    $this->_value = false;
    $this->setIsDefaultValue(true);
    return true;
  }

  public function get(): bool {
    return $this->_value;
  }

  protected function importFromBool(bool $value): bool {
    $this->_value = $value;
    return true;
  }

  protected function importFromFloat(float $value): bool {
    // why the really pedantic if? because otherwise you are allowing things
    // like 1.05 == true || 0.5 == false
    if ($value == 0.0) {
      $this->_value = false;
      return true;
    } else if ($value == 1.0) {
      $this->_value = true;
      return true;
    }
    throw new FailedToImportFromFloatException('value='.$value);
  }

  protected function importFromInteger(int $value): bool {
    if ($value == 0) {
      $this->_value = false;
      return true;
    } else if ($value == 1) {
      $this->_value = true;
      return true;
    }
    throw new FailedToImportFromIntegerException('value='.$value);
  }

  protected function importFromString(string $value): bool {

    // 1/true
    if (preg_match('/^(1|true)$/i', $value)) {
      $this->_value = true;
      return true;
    }

    // 0/false
    if (preg_match('/^(0|false)$/i', $value)) {
      $this->_value = false;
      return true;
    }

    throw new FailedToImportFromStringException('value='.$value);

  }

}
