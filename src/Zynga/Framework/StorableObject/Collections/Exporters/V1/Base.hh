<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\Exporters\V1;

use
  Zynga\Framework\StorableObject\Collections\V1\Interfaces\StorableCollection
;

use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldsGeneric;
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;
use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

use Zynga\Framework\Exception\V1\Exception;

/**
 * Base class for collection exporters. This class should be inherited
 * and each collection should have their own concrete exporter.
 */
abstract class Base<Tv> implements ExportInterface {

  public function __construct(private StorableCollection<Tv> $collection) {}

  public function asJSON(
    ?string $parentFieldName = null,
    bool $sorted = false,
  ): string {
    try {

      if ($this->collection->count() == 0) {
        throw new NoFieldsFoundException(
          'class='.get_class($this->collection),
        );
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

      $items = $this->collection->items();

      foreach ($items as $storableItem) {

        // We can skip the value if the item is still in default state.
        list($isRequired, $isDefaultValue) =
          FieldsGeneric::getIsRequiredAndIsDefaultValue($storableItem);

        if ($isDefaultValue === true) {
          continue;
        }

        $value = null;
        if ($storableItem instanceof StorableObjectInterface) {
          try {
            $value = $storableItem->export()->asJSON(null, $sorted);
          } catch (NoFieldsFoundException $e) {
            // no fields to export, fine.. skip this element.
            $value = null;
          } catch (Exception $e) {
            throw $e;
          }
        } else if ($storableItem instanceof TypeInterface) {
          $value = json_encode($storableItem->get());
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
    throw new OperationNotSupportedException(__METHOD__.' not supported');
  }

  public function asBinary(): string {
    throw new OperationNotSupportedException(__METHOD__.' not supported');
  }

}
