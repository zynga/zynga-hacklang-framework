<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Map\V2\Importer;

use Zynga\Framework\Dynamic\V1\DynamicClassCreation;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;

/**
 * Class for type collection importers. This class creates object of
 * the valueType the storableMap has specified.
 */
class Box<Tv> extends Base<Tv> {
  protected function getTypeFromItem(mixed $item): Tv {
    $type = DynamicClassCreation::createClassByNameGeneric(
      $this->valueType,
      Vector {},
    );

    try {
      $type->set($item);
      return $type;
    } catch (Exception $e) {
      $exception = new UnsupportedTypeException();
      $exception->setPrevious($e);
      throw $exception;
    }
  }
}
