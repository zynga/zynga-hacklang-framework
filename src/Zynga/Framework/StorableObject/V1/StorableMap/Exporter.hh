<?hh // strict

namespace Zynga\Framework\StorableObject\V1\StorableMap;

use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldsGeneric;
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;

use Zynga\Framework\StorableObject\V1\Exporter\Protobuf;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

use Zynga\Framework\Exception\V1\Exception;

use Zynga\Framework\StorableObject\V1\StorableMap;

class Exporter<Tv> implements ExportInterface {

  private StorableMap<Tv> $_object;

  public function __construct(StorableMap<Tv> $object) {
    $this->_object = $object;
  }

  public function asJSON(
    ?string $parentFieldName = null,
    bool $sorted = false,
  ): string {
    try {
      $map = $this->_object->getMap();
      if ($map->keys()->count() == 0) {
        throw new NoFieldsFoundException('class='.get_class($this->_object));
      }

      $payload = '';

      if ($parentFieldName !== null) {
        $payload .= json_encode($parentFieldName).':';
      }

      $payload .= '{';

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
          } catch (NoFieldsFoundException $e) {
            // no fields to export, fine.. skip this element.
            $value = null;
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

      $payload .= '}';

      return $payload;

    } catch (Exception $e) {
      throw $e;
    }
  }

  public function asMap(): Map<string, mixed> {

    try {

      $map = $this->_object->getMap();

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
    return '';
    // $proto = new Protobuf();
    // return $proto->asBinary($this->_object);
  }

}
