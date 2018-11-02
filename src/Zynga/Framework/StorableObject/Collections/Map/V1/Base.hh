<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Map\V1;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;
use Zynga\Framework\StorableObject\V1\Fields;
use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldsGeneric;

use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;

use Zynga\Framework\StorableObject\Collections\Map\V1\Exporter;
use Zynga\Framework\StorableObject\Collections\Map\V1\Importer;

use
  Zynga\Framework\StorableObject\Collections\V1\Interfaces\StorableMapCollection
;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;

class Base<Tk, Tv> implements StorableMapCollection<Tk, Tv> {
  private bool $isRequired;
  private Map<Tk, Tv> $map;
  private bool $isDefaultValue;

  public function __construct() {
    $this->isRequired = false;
    $this->isDefaultValue = true;
    $this->map = Map {};
  }

  public function clear(): bool {
    $this->map->clear();
    $this->isDefaultValue = true;
    return true;
  }

  public function isEmpty(): bool {
    if ($this->map->count() == 0) {
      return true;
    }
    return false;
  }

  public function count(): int {
    return $this->map->count();
  }

  public function items(): Map<Tk, Tv> {
    return $this->map;
  }

  public function add(Tk $k, Tv $v): bool {
    if ($this->map->containsKey($k) === false) {
      $this->map->set($k, $v);
      return true;
    }
    return false;
  }

  public function addAll(?Traversable<Pair<Tk, Tv>> $iterable): Map<Tk, Tv> {
    return $this->map->addAll($iterable);
  }

  public function get(Tk $key): ?Tv {

    if ($this->map->containsKey($key) === true) {
      return $this->map[$key];
    }

    return null;

  }

  public function at(Tk $key): Tv {
    return $this->map->at($key);
  }

  public function toArray(): array<Tk, Tv> {
    return $this->map->toArray();
  }

  public function setIsDefaultValue(bool $tf): bool {
    $this->isDefaultValue = $tf;
    return true;
  }

  public function isDefaultValue(): (bool, Vector<string>) {

    $defaultFields = Vector {};

    $hadNonDefault = false;

    foreach ($this->map as $key => $value) {

      list($f_isRequired, $f_isDefaultValue) =
        FieldsGeneric::getIsRequiredAndIsDefaultValue($value);

      if ($f_isDefaultValue === true) {
        if (is_string($key)) {
          $defaultFields[] = $key;
        } else {
          $defaultFields[] = strval($key);
        }
      } else {
        $hadNonDefault = true;
      }

    }

    $isDefaultValue = $this->isDefaultValue;

    if ($hadNonDefault === true) {
      $isDefaultValue = false;
    }

    return tuple($isDefaultValue, $defaultFields);

  }

  public function set(Tk $key, Tv $value): bool {
    $this->map[$key] = $value;
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
    $this->isRequired = $isRequired;
    return true;
  }

  public function getIsRequired(): bool {
    return $this->isRequired;
  }

}
