<?hh // strict

namespace Zynga\Framework\StorableObject\V1;

use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\StorableObject\V1\Fields\Generic as FieldsGeneric;

use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

use Zynga\Framework\Exception\V1\Exception;

class Fields implements FieldsInterface {
  private StorableObjectInterface $_object;

  public function __construct(StorableObjectInterface $object) {
    $this->_object = $object;
  }

  public function getTypedField(string $fieldName): TypeInterface {
    return FieldsGeneric::getTypedField($this->_object, $fieldName);
  }

  public function getForObject(): Map<string, mixed> {
    return FieldsGeneric::getForObject($this->_object);
  }

  public function getFieldsAndTypesForObject(): Map<string, string> {
    return FieldsGeneric::getFieldsAndTypesForObject($this->_object);
  }

  public function getRequiredFieldsWithDefaultValues(
    string $context = '',
  ): Vector<string> {

    try {

      $fields = $this->getForObject();

      $defaultFields = Vector {};

      if ($fields->count() == 0) {
        return $defaultFields;
      }

      foreach ($fields as $fieldName => $field) {

        // is it a type?
        if (!$field instanceof TypeInterface) {
          continue;
        }

        // do we require this field?
        if ($field->getIsRequired() !== true) {
          continue;
        }

        // pull the isDefaultValue state, and the
        list($isDefaultValue, $defaultValues) = $field->isDefaultValue();

        if ($isDefaultValue === true) {
          $errorString = '';
          if ($context != '') {
            $errorString .= $context.'.';
          }
          $errorString .= $fieldName;
          $defaultFields[] = $errorString;
        }

      }

      foreach ($fields as $fieldName => $field) {

        // is it a type?
        if (!$field instanceof StorableObjectInterface) {
          continue;
        }

        // do we require this field?
        if ($field->getIsRequired() !== true) {
          continue;
        }

        // pull the isDefaultValue state, and the
        $newContext = '';

        // prepend our context if any.
        if ($context != '') {
          $newContext .= $context.'.';
        }

        $newContext .= $fieldName;

        $defaultValues =
          $field->fields()->getRequiredFieldsWithDefaultValues($newContext);

        foreach ($defaultValues as $defaultValue) {
          $defaultFields[] = $defaultValue;
        }

      }

      return $defaultFields;

    } catch (Exception $e) {
      throw $e;
    }
  }

}
