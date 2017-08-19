<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Base as BaseBox;

use Zynga\Framework\Exception\V1\Exception;

use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromIntegerException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromFloatException;

class Int32Box extends BaseBox {
  private int $_value;

  public function __construct() {
    parent::__construct();
    $this->_value = 0;
  }

  public function reset(): bool {
    $this->_value = 0;
    $this->setIsDefaultValue(true);
    return true;
  }

  public function get(): int {
    return $this->_value;
  }

  <<__Override>>
  protected function importFromBool(bool $value): bool {
    if ($value === true) {
      $this->_value = 1;
    } else {
      $this->_value = 0;
    }
    return true;
  }

  <<__Override>>
  protected function importFromFloat(float $value): bool {
    if ($this->doesFloatHaveFraction($value) !== true) {
      $value = intval($value);
      return $this->importFromInteger($value);
    }
    throw new FailedToImportFromFloatException('value='.$value);
  }

  <<__Override>>
  protected function importFromInteger(int $value): bool {
    if ($value >= -2147483648 && $value <= 2147483647) {
      $this->_value = $value;
      return true;
    }
    throw new FailedToImportFromIntegerException('value='.$value);
  }

  public function doesFloatHaveFraction(float $value): bool {
    $fraction = $value - floor($value);
    if ($fraction > 0) {
      return true;
    }
    return false;
  }

  <<__Override>>
  protected function importFromString(string $value): bool {

    if (is_numeric($value) === true &&
        $this->doesFloatHaveFraction(floatval($value)) !== true) {
      return $this->set(intval($value));
    }

    throw new FailedToImportFromStringException('value='.$value);

  }

}
