<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Map\V2\Importer;

use Zynga\Framework\StorableObject\Collections\Map\V2\Base as StorableMap;
use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;

use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;
use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;

abstract class Base<Tv> implements ImportInterface {
  public function __construct(
    protected StorableMap<Tv> $collection,
    protected string $valueType,
  ) {}

  public function fromVector(Vector<mixed> $data): bool {
    $this->collection->clear();

    $keys = $data->keys();

    foreach ($keys as $key) {
      $item = $data->at($key);
      $storable = $this->getTypeFromItem($item);
      $this->collection->set(strval($key), $storable);
    }

    return true;
  }

  public function fromMap(Map<string, mixed> $data): bool {
    $this->collection->clear();

    foreach ($data as $key => $item) {
      $storable = $this->getTypeFromItem($item);
      $key = strval($key);
      $this->collection->set($key, $storable);
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

  public function fromBinary(string $payload): bool {
    throw new OperationNotSupportedException(
      'method='.__METHOD__.' not supported',
    );
  }

  protected function isArrayKeyValuePair(array<mixed> $array): bool {
    return array_values($array) !== $array;
  }

  /**
   * Factory method for the value item the storableMap encapsulates.
   * This should return the item type with the deserialized payload.
   * @param $item Unknown type that must be deserialized
   */
  abstract protected function getTypeFromItem(mixed $item): Tv;
}
