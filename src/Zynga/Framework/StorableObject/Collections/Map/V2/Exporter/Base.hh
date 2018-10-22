<?hh // strict
namespace Zynga\Framework\StorableObject\Collections\Map\V2\Exporter;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\StorableObject\Collections\Map\V2\Base as StorableMap;
use
  Zynga\Framework\StorableObject\V1\Exceptions\OperationNotSupportedException
;
use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldsGeneric;
use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

class Base<Tv> implements ExportInterface {
  public function __construct(private StorableMap<Tv> $object) {}

  public function asJSON(
    ?string $parentFieldName = null,
    bool $sorted = false,
  ): string {
    try {
      $map = $this->object->items();
      if ($map->keys()->count() == 0) {
        return '{}';
      }

      $payload = '{';

      if ($parentFieldName !== null) {
        $payload .= json_encode($parentFieldName).':{';
      }

      $firstValue = true;

      $map->keys();
      if ($sorted) {
        ksort($map);
      }

      foreach ($map as $fieldName => $field) {
        // We can skip the value if the item is still in default state.
        list($isRequired, $isDefaultValue) =
          FieldsGeneric::getIsRequiredAndIsDefaultValue($field);

        if ($isDefaultValue === true) {
          continue;
        }

        $value = null;

        if ($field instanceof TypeInterface) {
          $value = json_encode($field->get());
        } else if ($field instanceof StorableObjectInterface) {
          try {
            $value = $field->export()->asJSON(null, $sorted);
          } catch (Exception $e) {
            throw $e;
          }
        }

        if ($value !== null) {

          if ($firstValue === false) {
            $payload .= ',';
          }

          $payload .= json_encode($fieldName).':';
          $payload .= $value;

          $firstValue = false;
        }

      }

      if ($parentFieldName != null) {
        $payload .= '}';
      }

      $payload .= '}';

      return $payload;

    } catch (Exception $e) {
      throw $e;
    }
  }

  public function asMap(): Map<string, mixed> {

    try {

      $map = $this->object->items();

      $payload = Map {};

      if ($map->count() == 0) {
        return $payload;
      }

      foreach ($map as $fieldName => $field) {

        $value = null;

        if ($field instanceof TypeInterface) {
          $value = $field->get();
        } else if ($field instanceof StorableObjectInterface) {
          $value = $field->export()->asMap();
        }

        if ($value !== null) {
          $payload[$fieldName] = $value;
        }

      }

      return $payload;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function asBinary(): string {
    throw new OperationNotSupportedException(__METHOD__.' not supported');
  }
}
