<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Vector\V1;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;
use Zynga\Framework\StorableObject\V1\Fields;
use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldsGeneric;
use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;

use
  Zynga\Framework\StorableObject\Collections\V1\Interfaces\StorableCollection
;

/**
 * This collection lets us be able to serialize/deserialize storables.
 * Note that you must give collections type hints in order for deserialization to work.
 * The reason is because at runtime type erasure forces us to loss information as to what
 * type a collection contains.
 * Check out this page for more info: https://docs.hhvm.com/hack/generics/erasure
 * @see Zynga\Framework\StorableObject\Collections\V1\Interfaces\StorableCollection for more info
 */
class Base<Tv as StorableObjectInterface> implements StorableCollection<Tv> {
  private Vector<Tv> $vector;
  private bool $isRequired;
  private ?bool $isDefaultValue;

  public function __construct(private classname<Tv> $rawType) {
    $this->isRequired = false;
    $this->isDefaultValue = null;
    $this->vector = Vector {};
  }

  public function isEmpty(): bool {
    return $this->vector->isEmpty();
  }

  public function count(): int {
    return $this->vector->count();
  }

  public function items(): Iterable<Tv> {
    return $this->vector->items();
  }

  public function clear(): void {
    $this->vector->clear();
  }

  public function add(Tv $item): this {
    $this->vector->add($item);
    return $this;
  }

  public function addAll(?Traversable<Tv> $traversable): this {
    $this->vector->addAll($traversable);
    return $this;
  }

  public function setAll(?KeyedTraversable<int, Tv> $traversable): this {
    $this->vector->setAll($traversable);
    return $this;
  }

  public function removeKey(int $key): this {
    $this->vector->removeKey($key);
    return $this;
  }

  /**
   * @throws OutOfBoundsException
   */
  public function at(int $key): Tv {
    return $this->vector->at($key);
  }

  public function containsKey<Tu super int>(Tu $key): bool {
    return $this->vector->containsKey($key);
  }

  public function fields(): FieldsInterface {
    $fields = new Fields($this);
    return $fields;
  }

  public function import(): ImportInterface {
    $importer = new Importer($this, $this->rawType);
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

  public function setIsDefaultValue(bool $tf): bool {
    $this->isDefaultValue = $tf;
    return true;
  }

  public function isDefaultValue(): (bool, Vector<string>) {
    $defaultFields = Vector {};

    if ($this->isDefaultValue !== null) {
      return tuple($this->isDefaultValue, $defaultFields);
    }

    $offset = 0;
    $hadNonDefault = false;

    foreach ($this->vector as $value) {
      list($f_isRequired, $f_isDefaultValue) =
        FieldsGeneric::getIsRequiredAndIsDefaultValue($value);

      if ($f_isDefaultValue === true) {
        $defaultFields[] = '['.strval($offset).']';
      } else {
        $hadNonDefault = true;
      }

      $offset++;
    }

    $isDefaultValue = true;

    if ($hadNonDefault === true) {
      $isDefaultValue = false;
    }

    return tuple($isDefaultValue, $defaultFields);
  }
}
