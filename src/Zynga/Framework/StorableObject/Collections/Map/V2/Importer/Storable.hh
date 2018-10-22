<?hh // strict
namespace Zynga\Framework\StorableObject\Collections\Map\V2\Importer;

use Zynga\Framework\Dynamic\V1\DynamicClassCreation;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;

/**
 * Class for collection importers. This class creates object of the
 * valueType the storableMap has specified. Since valueType is of 
 * storable nature we expect the payload to be a key value pairs to
 * be deserialize.
 */
class Storable<Tv> extends Base<Tv> {
  protected function getTypeFromItem(mixed $item): Tv {

    $storable = DynamicClassCreation::createClassByNameGeneric(
      $this->valueType,
      Vector {},
    );

    if (is_array($item)) {
      if ($this->isArrayKeyValuePair($item)) {
        $storable->import()->fromMap(new Map($item));
      } else {
        $storable->import()->fromVector(new Vector($item));
      }

      return $storable;
    }

    if ($item instanceof Vector) {
      $storable->import()->fromVector($item);
      return $storable;
    }

    if ($item instanceof Map) {
      $storable->import()->fromMap($item);
      return $storable;
    }

    throw new UnsupportedTypeException(
      'Unable to import item. item='.json_encode($item),
    );
  }
}
