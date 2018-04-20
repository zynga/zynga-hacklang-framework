<?hh //strict

namespace Zynga\Framework\StorableObject\Collections\Vector\V1\Importer;

use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;
use
  Zynga\Framework\StorableObject\Collections\V1\Interfaces\StorableCollection
;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;

/**
 * Base class for importers. Importers of specific types must inherit from this class.
 */
abstract class Base<Tv> implements ImportInterface {
  public function __construct(
    protected StorableCollection<Tv> $collection,
    protected string $rawType,
  ) {}

  public function fromVector(Vector<mixed> $data): bool {
    $this->collection->clear();

    foreach ($data as $item) {
      $storable = $this->getTypeFromItem($item);
      $this->collection->add($storable);
    }

    return true;
  }

  public function fromMap(Map<string, mixed> $data): bool {

    $this->collection->clear();

    foreach ($data as $key => $item) {
      $storable = $this->getTypeFromItem($item);
      $this->collection->add($storable);
    }

    return true;
  }

  public function fromJSON(string $payload): bool {
    $decodedJson = json_decode($payload, true);
    if (is_array($decodedJson) === false) {
      throw new UnsupportedTypeException(
        'Payload is not valid JSON. payload='.$payload,
      );
    }

    if ($this->isArrayKeyValuePair($decodedJson)) {
      $this->fromMap(new Map($decodedJson));
    } else {
      $this->fromVector(new Vector($decodedJson));
    }
    return true;
  }

  /**
   * Factory method for the item the collection encapsulates.
   * This should return the item type with the deserialized payload.
   * @param $item Unknown type that must be deserialized
   */
  abstract protected function getTypeFromItem(mixed $item): Tv;

  protected function isArrayKeyValuePair(array<mixed> $array): bool {
    return array_values($array) !== $array;
  }
}
