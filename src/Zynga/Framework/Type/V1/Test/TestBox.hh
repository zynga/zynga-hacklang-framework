<?hh // strict

namespace Zynga\Framework\Type\V1\Test;

use Zynga\Framework\Type\V1\Base as BaseBox;

class TestBox extends BaseBox {
  private mixed $_value;

  public function __construct(mixed $value = null) {

    parent::__construct();

    $this->_value = null;
    $this->setIsDefaultValue(true);

    if ($value !== null) {
      $this->_value = $value;
    }

  }

  <<__Override>>
  public function reset(): bool {
    $this->_value = null;
    $this->setIsDefaultValue(true);
    return true;
  }

  <<__Override>>
  public function get(): mixed {
    return $this->_value;
  }

  <<__Override>>
  protected function importFromBool(bool $value): bool {
    $this->_value = $value;
    return true;
  }

  <<__Override>>
  protected function importFromFloat(float $value): bool {
    $this->_value = $value;
    return true;
  }

  <<__Override>>
  protected function importFromInteger(int $value): bool {
    $this->_value = $value;
    return true;
  }

  <<__Override>>
  protected function importFromString(string $value): bool {
    $this->_value = $value;
    return true;
  }
}

