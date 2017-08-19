<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Base as BaseBox;

use Zynga\Framework\Exception\V1\Exception;

use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromFloatException;

class Int64Box extends BaseBox {
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

  <<__Override>>
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

    // --
    // PHP is incapable of doing comparisions on floats correctly. So we convert
    // the value to a string and then the comparison works.
    // --
    $min_ret = strval($value) < strval(PHP_INT_MIN);
    $max_ret = strval($value) > strval(PHP_INT_MAX);

    if ($min_ret == 1) {
      throw new FailedToImportFromFloatException('value='.$value);
    }

    if ($max_ret == 1) {
      throw new FailedToImportFromFloatException('value='.$value);
    }

    if ($this->doesFloatHaveFraction($value) !== true) {
      $value = intval($value);
      return $this->importFromInteger($value);
    }

    throw new FailedToImportFromFloatException('value='.$value);

  }

  public function doesFloatHaveFraction(float $value): bool {
    $fraction = $value - floor($value);
    if ($fraction > 0) {
      return true;
    }
    return false;
  }

  <<__Override>>
  protected function importFromInteger(int $value): bool {

    // --
    // Why no error trap here for > < INT_(MIN|MAX)? because with types it won't
    // get to here otherwise /yay
    // --
    $this->_value = $value;

    return true;

  }

  protected function importFromString(string $value): bool {

    if (is_numeric($value) === true &&
        $this->doesFloatHaveFraction(floatval($value)) !== true) {
      return $this->set(intval($value));
    }

    throw new FailedToImportFromStringException('value='.$value);

  }

}
