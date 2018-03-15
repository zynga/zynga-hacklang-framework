<?hh // strict

namespace Zynga\Framework\StorableObject\V1;

use JsonSerializable;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use
  Zynga\Framework\StorableObject\V1\Exceptions\MissingKeyFromImportDataException
;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;
use
  Zynga\Framework\StorableObject\V1\Exceptions\PropertyUnavailableException
;
use
  Zynga\Framework\StorableObject\V1\Exceptions\ExpectedFieldCountMismatchException
;

use Zynga\Framework\StorableObject\V1\SupportedTypes;

use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;

use Zynga\Framework\StorableObject\V1\Object\Importer;
use Zynga\Framework\StorableObject\V1\Object\Exporter;

use Zynga\Type\V1\Interfaces\TypeInterface;

use Zynga\Framework\StorableObject\V1\Fields;
use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldsGeneric;

use Zynga\Framework\Exception\V1\Exception;

abstract class Base implements StorableObjectInterface, JsonSerializable {
  private bool $_isRequired;
  private ?bool $_isDefaultValue;

  public function __construct() {
    // --
    // As hack requires you to not use $this-><func> until the init is overwith
    // we can make the assumption the default value is going to be set for the
    // object
    // --
    $this->_isRequired = false;
    $this->_isDefaultValue = null;
  }

  public function fields(): FieldsInterface {
    $fields = new Fields($this);
    return $fields;
  }

  public function import(): ImportInterface {
    $importer = new Importer($this);
    return $importer;
  }

  public function export(): ExportInterface {
    $exporter = new Exporter($this);
    return $exporter;
  }

  public function setIsRequired(bool $isRequired): bool {
    $this->_isRequired = $isRequired;
    return true;
  }

  public function getIsRequired(): bool {
    return $this->_isRequired;
  }

  public function setIsDefaultValue(bool $tf): bool {
    $this->_isDefaultValue = $tf;
    return true;
  }

  public function isDefaultValue(): (bool, Vector<string>) {

    $defaultFields = Vector {};

    if ($this->_isDefaultValue !== null) {
      return tuple($this->_isDefaultValue, $defaultFields);
    }

    $fields = $this->fields()->getForObject();

    foreach ($fields as $fieldName => $field) {

      list($f_isRequired, $f_isDefaultValue) =
        FieldsGeneric::getIsRequiredAndIsDefaultValue($field);

      // echo json_encode(array(get_class($this), $fieldName, $f_isDefaultValue)) . "\n";

      if ($f_isDefaultValue === true) {
        $defaultFields[] = $fieldName;
      }

    }

    $isDefaultValue = true;

    // --
    // if any of the fields or child fields are not the default,
    // we are okay marking this object as not default.
    // --
    if ($defaultFields->count() != $fields->count()) {
      $isDefaultValue = false;
    }

    return tuple($isDefaultValue, $defaultFields);

  }

  public function jsonSerialize(): mixed {
    return $this->export()->asMap()->toArray();
  }

}
