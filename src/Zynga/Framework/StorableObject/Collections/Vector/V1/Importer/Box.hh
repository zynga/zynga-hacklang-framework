<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Vector\V1\Importer;

use
  Zynga\Framework\StorableObject\Collections\V1\Interfaces\StorableCollection
;
use Zynga\Framework\StorableObject\Collections\Vector\V1\Importer\Base;
use Zynga\Framework\Dynamic\V1\DynamicClassCreation;

use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\StorableObject\V1\Fields;

use
  Zynga\Framework\StorableObject\V1\Exceptions\ExpectedFieldCountMismatchException
;
use
  Zynga\Framework\StorableObject\V1\Exceptions\MissingKeyFromImportDataException
;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;
use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

use Zynga\Framework\Exception\V1\Exception;

/**
 * Base class for type collection importers. This class should be inherited
 * and each collection should have their own concrete importer.
 * Example:
 * // For StringBox, we can deserialize the payload safely with the below JSON
 * {["myString2"]}
 */
class Box<Tv> extends Base<Tv> {
  /**
   * When deserializing items for a storable object,
   * we expect each item to not be key value pairs. They should be
   * deserializable given the box type.
   */
  protected function getTypeFromItem(mixed $item): Tv {
    $type = DynamicClassCreation::createClassByNameGeneric(
      $this->rawType,
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

  public function fromBinary(string $payload): bool {
    throw new OperationNotSupportedException(
      'method='.__METHOD__.' not supported',
    );
  }

}
