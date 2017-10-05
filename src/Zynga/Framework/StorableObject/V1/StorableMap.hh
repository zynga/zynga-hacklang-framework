<?hh // strict

namespace Zynga\Framework\StorableObject\V1;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;

use Zynga\Framework\StorableObject\V1\StorableMap\Importer;
use Zynga\Framework\StorableObject\V1\StorableMap\Exporter;
use Zynga\Framework\StorableObject\V1\Fields;

use Zynga\Type\V1\Interfaces\TypeInterface;

use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldsGeneric;

use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;

class StorableMap<Tv> implements StorableObjectInterface {
  private bool $_isRequired;
  private Map<string, Tv> $_map;
  private bool $_isDefaultValue;

  public function __construct() {
    $this->_isRequired = false;
    $this->_isDefaultValue = true;
    $this->_map = Map {};
  }

  public function reset(): bool {
    $this->_map->clear();
    $this->_isDefaultValue = true;
    return true;
  }

  public function count(): int {
    return $this->_map->count();
  }

  public function getMap(): Map<string, Tv> {
    return $this->_map;
  }

  public function get(string $key): ?Tv {

    if ($this->_map->containsKey($key) === true) {
      return $this->_map[$key];
    }

    return null;

  }

  public function setIsDefaultValue(bool $tf): bool {
    $this->_isDefaultValue = $tf;
    return true;
  }

  public function isDefaultValue(): (bool, Vector<string>) {

    $defaultFields = Vector {};

    $hadNonDefault = false;

    foreach ($this->_map as $key => $value) {

      list($f_isRequired, $f_isDefaultValue) =
        FieldsGeneric::getIsRequiredAndIsDefaultValue($value);

      if ($f_isDefaultValue === true) {
        $defaultFields[] = $key;
      } else {
        $hadNonDefault = true;
      }

    }

    $isDefaultValue = $this->_isDefaultValue;

    if ($hadNonDefault === true) {
      $isDefaultValue = false;
    }

    return tuple($isDefaultValue, $defaultFields);
  }

  public function set(string $key, Tv $value): bool {
    $this->_map[$key] = $value;
    $this->setIsDefaultValue(false);
    return true;
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

}
