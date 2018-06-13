<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;

use
  Zynga\Framework\Swagger\V2\Swagger\DefinitionReference\Fields as DefinitionReferenceFields
;
use
  Zynga\Framework\Swagger\V2\Swagger\DefinitionReference\Import as DefinitionReferenceImport
;
use
  Zynga\Framework\Swagger\V2\Swagger\DefinitionReference\Export as DefinitionReferenceExport
;

class DefinitionReference implements TypeInterface, StorableObjectInterface {
  private string $_className;
  private bool $_isRequired;
  private bool $_isDefaultValue;

  public function __construct() {
    $this->_className = '';
    $this->_isRequired = false;
    $this->_isDefaultValue = true;
  }

  public function reset(): bool {
    $this->_className = '';
    $this->setIsDefaultValue(true);
    return true;
  }

  public function getIsRequired(): bool {
    return $this->_isRequired;
  }

  public function setIsRequired(bool $isRequired): bool {
    $this->_isRequired = $isRequired;
    return true;
  }

  public function isDefaultValue(): (bool, Vector<string>) {
    $vec = Vector {};
    return tuple($this->_isDefaultValue, $vec);
  }

  public function setIsDefaultValue(bool $tf): bool {
    $this->_isDefaultValue = $tf;
    return true;
  }

  public function setDefaultValue(mixed $value): bool {
    if (is_string($value)) {
      $this->_className = $value;
      return true;
    }
    return false;
  }

  public function get(): mixed {
    return $this->_className;
  }

  public function set(mixed $value): bool {
    if (is_string($value)) {
      $this->_className = $value;
      $this->_isDefaultValue = false;
      return true;
    }
    return false;
  }

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

  public function fields(): FieldsInterface {
    $fields = new DefinitionReferenceFields();
    return $fields;
  }

  public function import(): ImportInterface {
    $import = new DefinitionReferenceImport();
    return $import;
  }

  public function export(): ExportInterface {
    $export = new DefinitionReferenceExport($this);
    return $export;
  }

}
