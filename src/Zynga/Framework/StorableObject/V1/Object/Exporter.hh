<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Object;

use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldsGeneric;
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;
use
  Zynga\Framework\StorableObject\V1\Exceptions\MissingDataFromExportDataException
;

use Zynga\Framework\StorableObject\V1\Exporter\Protobuf;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Legacy\V1\Log;

use Zynga\Framework\Exception\V1\Exception;

class Exporter implements ExportInterface {

  private StorableObjectInterface $_object;

  public function __construct(StorableObjectInterface $object) {
    $this->_object = $object;
  }

  public function asJSON(
    ?string $parentFieldName = null,
    bool $sorted = false,
  ): string {
    try {
      $fields = $this->_object->fields()->getForObject();

      $requiredFields =
        $this->_object->fields()->getRequiredFieldsWithDefaultValues();
      if ($requiredFields->count() > 0) {
        $message = '';
        $message .= 'fields='.json_encode($requiredFields);
        throw new MissingDataFromExportDataException($message);
      }

      $payload = '';

      if ($parentFieldName !== null) {
        $payload .= json_encode($parentFieldName).':';
      }

      $payload .= '{';

      $firstField = true;

      if ($sorted) {
        ksort(&$fields);
      }

      foreach ($fields as $fieldName => $field) {
        list($isRequired, $isDefaultValue) =
          FieldsGeneric::getIsRequiredAndIsDefaultValue($field);
        //echo "fieldName=$fieldName\n";
        //var_dump($isRequired);
        //var_dump($isDefaultValue);
        //var_dump($field);

        // --
        // We don't dump non-changed values into our JSON
        // --
        if ($isDefaultValue === true) {
          continue;
        }

        $value = null;

        if ($field instanceof TypeInterface &&
            !$field instanceof StorableObjectInterface) {
          $value = json_encode($fieldName).':';
          $value .= json_encode($field->get());
        } else if ($field instanceof StorableObjectInterface) {
          try {
            $value = $field->export()->asJSON($fieldName, $sorted);
          } catch (NoFieldsFoundException $e) {
            // we do not add anything, as there is nothing to add.
          } catch (Exception $e) {
            throw new Exception(
              'fieldName='.
              $fieldName.
              ' e='.
              get_class($e).
              ' message='.
              $e->getMessage(),
            );
          }
        }

        if ($value !== null && $value != '') {
          // only add the comma if it's needed as we have a value.
          if ($firstField === false) {
            $payload .= ',';
          }
          // add the value to the stack
          $payload .= $value;
          $firstField = false;
        }

      }

      $payload .= '}';

      if ($firstField === true) {
        return '{}';
      }

      return $payload;

    } catch (Exception $e) {
      throw $e;
    }
  }

  public function asMap(): Map<string, mixed> {

    $fields = $this->_object->fields()->getForObject();

    if ($fields->count() == 0) {
      throw new NoFieldsFoundException('class='.get_class($this->_object));
    }

    $payload = Map {};

    foreach ($fields as $fieldName => $field) {

      $value = null;

      if ($field instanceof TypeInterface) {
        $value = $field->get();
      } else if ($field instanceof StorableObjectInterface) {
        try {
          $value = $field->export()->asMap();
        } catch (Exception $e) {
          throw new Exception(
            'fieldName='.
            $fieldName.
            ' e='.
            get_class($e).
            ' message='.
            $e->getMessage(),
          );
        }
      }

      $payload[$fieldName] = $value;

    }

    return $payload;

  }

  public function asBinary(): string {

    $proto = new Protobuf();
    return $proto->asBinary($this->_object);

  }

}
