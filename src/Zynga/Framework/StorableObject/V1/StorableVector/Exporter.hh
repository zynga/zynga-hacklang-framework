<?hh // strict

namespace Zynga\Framework\StorableObject\V1\StorableVector;

use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldsGeneric;
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;
use Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException;

use Zynga\Framework\StorableObject\V1\Exporter\Protobuf;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

use Zynga\Framework\Exception\V1\Exception;

use Zynga\Framework\StorableObject\V1\StorableVector;

use Zynga\Legacy\V1\Log;

class Exporter<Tv> implements ExportInterface {

  private StorableVector<Tv> $_object;

  public function __construct(StorableVector<Tv> $object) {
    $this->_object = $object;
  }

  public function asJSON(?string $parentFieldName = null, bool $sorted = false): string {
    try {

      $vec = $this->_object->getVector();

      if ($vec->count() == 0) {
        throw new NoFieldsFoundException('class='.get_class($this->_object));
      }

      $stack = '';

      if ($parentFieldName !== null) {
        $stack .= json_encode($parentFieldName).':';
      }

      // --
      // We serialize the individual items off into a json array, no need to make
      // a temporary array and then run json encode on it as the vector / array
      // type is so simplistic and the child storable objects will handle the
      // heavy lifting of serialization.
      // --

      $stack .= '[';

      $firstItem = true;
      foreach ($vec as $storableItem) {

        // We can skip the value if the item is still in default state.
        list($isRequired, $isDefaultValue) =
          FieldsGeneric::getIsRequiredAndIsDefaultValue($storableItem);

        if ($isDefaultValue === true) {
          continue;
        }

        $value = null;
        if ($storableItem instanceof TypeInterface) {
          $value = json_encode($storableItem->get());
        } else if ($storableItem instanceof StorableObjectInterface) {
          try {
            $value = $storableItem->export()->asJSON(null, $sorted);
          } catch (NoFieldsFoundException $e) {
            // no fields to export, fine.. skip this element.
            $value = null;
          } catch (Exception $e) {
            throw $e;
          }
        }

        if ($value !== null) {
          if ($firstItem === false) {
            $stack .= ',';
          }
          $stack .= $value;
          $firstItem = false;
        }
      }

      $stack .= ']';
      return $stack;

    } catch (Exception $e) {
      throw $e;
    }
  }

  public function asMap(): Map<string, mixed> {
    throw new OperationNotSupportedException(
      'asMap-not-available-for-storableVector',
    );
  }

  public function asBinary(): string {
    return '';
  }

}
