<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Base as BaseBox;

use Zynga\Framework\Exception\V1\Exception;

use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromIntegerException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromFloatException;

class UInt16Box extends BaseBox {
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
      return $this->importFromInteger(1);
    }

    return $this->importFromInteger(0);
  }

  <<__Override>>
  protected function importFromFloat(float $value): bool {
    $fraction = $value - floor($value);
    if ($fraction == 0) {
      return $this->importFromInteger(intval($value));
    }

    throw new FailedToImportFromFloatException('value='.$value);
  }

  <<__Override>>
  protected function importFromInteger(int $value): bool {
    if ($value >= 0 && $value <= 65535) {
      $this->_value = $value;
      return true;
    }
    throw new FailedToImportFromIntegerException('value='.$value);
  }

  <<__Override>>
  protected function importFromString(string $value): bool {
    if (ctype_digit($value) === true) {
      return $this->importFromInteger(intval($value));
    }

    throw new FailedToImportFromStringException('value='.$value);
  }

}
