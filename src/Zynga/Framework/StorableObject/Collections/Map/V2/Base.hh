<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Map\V2;

use
  Zynga\Framework\StorableObject\Collections\Map\V2\Exporter\Base as Exporter
;
use
  Zynga\Framework\StorableObject\Collections\Map\V2\Importer\Box as BoxImporter
;
use
  Zynga\Framework\StorableObject\Collections\Map\V2\Importer\Storable as StorableImporter
;
use
  Zynga\Framework\StorableObject\Collections\V2\Interfaces\StorableMapCollection
;
use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;
use Zynga\Framework\StorableObject\V1\Fields;
use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldsGeneric;
use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

class Base<Tv> implements StorableMapCollection<Tv> {
  /**
   * Native Map only supports int keys or string keys
   * https://docs.hhvm.com/hack/reference/class/HH.Map/
   * We are forcing it to support only string as for data with
   * int keys we can make use of StorableVector
   */
  private Map<string, Tv> $map;
  private bool $isRequired;
  private bool $isDefaultValue;

  public function __construct(private classname<Tv> $valueType) {
    $interfaces = class_implements($valueType);
    if (array_key_exists(StorableObjectInterface::class, $interfaces) === false &&
        array_key_exists(TypeInterface::class, $interfaces) === false) {
      throw new OperationNotSupportedException(
        'Collection only support the following types for Map values:'.
        StorableObjectInterface::class.
        ', '.
        TypeInterface::class,
      );
    }

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

  public function items(): Map<string, Tv> {
    return $this->map;
  }

  public function add(Pair<string, Tv> $pair): this {
    $this->map->add($pair);
    return $this;
  }

  public function set(string $k, Tv $v): this {
    $this->map->set($k, $v);
    return $this;
  }

  public function addAll(?Traversable<Pair<string, Tv>> $iterable): this {
    $this->map->addAll($iterable);
    return $this;
  }

  public function setAll(?KeyedTraversable<string, Tv> $iterable): this {
    $this->map->setAll($iterable);
    return $this;
  }

  public function remove(string $key): this {
    $this->map->remove($key);
    return $this;
  }

  public function removeKey(string $key): this {
    $this->map->removeKey($key);
    return $this;
  }

  public function get(string $key): ?Tv {

    if ($this->map->containsKey($key) === true) {
      return $this->map[$key];
    }

    return null;

  }

  public function at(string $key): Tv {
    return $this->map->at($key);
  }

  public function containsKey<Tu super string>(Tu $key): bool {
    return $this->map->containsKey($key);
  }

  public function toArray(): array<string, Tv> {
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
        $defaultFields[] = $key;
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

  public function fields(): FieldsInterface {
    $fields = new Fields($this);
    return $fields;
  }

  public function import(): ImportInterface {
    $interfaces = class_implements($this->valueType);
    if (array_key_exists(StorableObjectInterface::class, $interfaces)) {
      return new StorableImporter($this, $this->valueType);
    }

    return new BoxImporter($this, $this->valueType);
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
