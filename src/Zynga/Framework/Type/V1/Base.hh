<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Type\V1\Exceptions\UnSupportedTypeException;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

abstract class Base implements TypeInterface {
  private bool $_isRequired;
  private bool $_isDefaultValue;

  public function __construct() {
    // --
    // As hack requires you to not use $this-><func> until the init is overwith
    // we can make the assumption the default value is going to be set for the
    // object
    // --
    $this->_isRequired = false;
    $this->_isDefaultValue = true;
  }

  <<__Override>>
  public function setIsRequired(bool $isRequired): bool {
    $this->_isRequired = $isRequired;
    return true;
  }

  <<__Override>>
  public function getIsRequired(): bool {
    return $this->_isRequired;
  }

  <<__Override>>
  public function set(mixed $value): bool {
    try {
      $this->import($value);
      $this->_isDefaultValue = false;
      return true;
    } catch (Exception $e) {
      throw $e;
    }
  }

  <<__Override>>
  public function isDefaultValue(): (bool, Vector<string>) {
    $vec = Vector {};
    return tuple($this->_isDefaultValue, $vec);
  }

  <<__Override>>
  public function setIsDefaultValue(bool $tf): bool {
    $this->_isDefaultValue = $tf;
    return true;
  }

  <<__Override>>
  public function setDefaultValue(mixed $value): bool {
    try {
      $this->import($value);
      $this->_isDefaultValue = true;
      return true;
    } catch (Exception $e) {
      throw $e;
    }
  }

  <<__Override>>
  public function equals(mixed $value): bool {
    if ($value === null) {
      return false;
    }

    // This first line is because "$value instanceof self" will be true for
    // everything inheriting from Base.hh
    // What we REALLY want to do is check if $value is a subclass of
    // $this's class, not that $value is a subclass of Base.hh
    if (is_a($value, get_class($this)) &&
        // This tells the typechecker that the next line is kosher
        // because it doesn't understand that the above line means that
        // $value is a subclass of $this and so implements get()
        $value instanceof self) {
      return $value->get() === $this->get();
    }

    return false;
  }

  public abstract function get(): mixed;

  protected function import(mixed $value): bool {
    try {
      if (is_int($value)) {
        return $this->importFromInteger($value);
      } else if (is_string($value)) {
        return $this->importFromString($value);
      } else if (is_float($value)) {
        return $this->importFromFloat($value);
      } else if (is_bool($value)) {
        return $this->importFromBool($value);
      }
    } catch (Exception $e) {
      throw $e;
    }
    throw new UnSupportedTypeException('type='.gettype($value));
  }

  abstract public function reset(): bool;

  abstract protected function importFromString(string $value): bool;
  abstract protected function importFromInteger(int $value): bool;
  abstract protected function importFromFloat(float $value): bool;
  abstract protected function importFromBool(bool $value): bool;

}
