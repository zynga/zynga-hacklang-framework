<?hh // strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\PgFk\FkCache;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

// TODO: Exception cleanup
use \Exception;

class PgFk<TModel as PgModelInterface, TRow as PgRowInterface>
  implements TypeInterface {
  private classname<TModel> $_fkModel;
  private classname<TRow> $_fkRow;
  private TypeInterface $_wrappedType;

  public function __construct(
    TypeInterface $wrappedType,
    classname<TModel> $fkModel,
    classname<TRow> $fkRow,
  ) {
    $this->_wrappedType = $wrappedType;
    $this->_fkModel = $fkModel;
    $this->_fkRow = $fkRow;
  }

  public function getFkModel(): classname<TModel> {
    return $this->_fkModel;
  }

  public function getFkRow(): classname<TRow> {
    return $this->_fkRow;
  }

  public function fk(): ?PgRowInterface {
    try {
      return FkCache::getFk(
        $this->_fkModel,
        $this->_fkRow,
        $this->_wrappedType->get(),
      );
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function reset(): bool {
    try {
      return $this->_wrappedType->reset();
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function getIsRequired(): bool {
    try {
      return $this->_wrappedType->getIsRequired();
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function setIsRequired(bool $isRequired): bool {
    try {
      return $this->_wrappedType->setIsRequired($isRequired);
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function isDefaultValue(): (bool, Vector<string>) {
    try {
      return $this->_wrappedType->isDefaultValue();
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function setIsDefaultValue(bool $tf): bool {
    try {
      return $this->_wrappedType->setIsDefaultValue($tf);
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function setDefaultValue(mixed $value): bool {
    try {
      return $this->_wrappedType->setDefaultValue($value);
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function get(): mixed {
    try {
      return $this->_wrappedType->get();
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function set(mixed $value): bool {
    try {
      return $this->_wrappedType->set($value);
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function equals(mixed $value): bool {
    try {
      return $this->_wrappedType->equals($value);
    } catch (Exception $e) {
      throw $e;
    }
  }

}
