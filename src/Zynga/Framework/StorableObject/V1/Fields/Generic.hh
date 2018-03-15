<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Fields;

use \ReflectionClass;

use Zynga\Framework\StorableObject\V1\Exceptions\InvalidObjectException;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

class Generic {

  public static function getTypedField(
    mixed $obj,
    string $fieldName,
  ): TypeInterface {
    try {

      if (!is_object($obj)) {
        throw new InvalidObjectException('passedType='.gettype($obj));
      }

      $reflectedClass = new ReflectionClass($obj);

      $properties = $reflectedClass->getProperties();

      foreach ($properties as $property) {

        if ($property->getName() == $fieldName) {

          $value = $property->getValue($obj);

          if ($value instanceof TypeInterface) {
            return $value;
          }

          throw new UnsupportedTypeException(
            'fieldName='.$fieldName.' is not a TypeInterface',
          );

        }

      }

      throw new NoFieldsFoundException(
        'fieldName='.$fieldName.' is not found',
      );

    } catch (Exception $e) {
      throw $e;
    }
  }

  public static function getForObject(mixed $obj): Map<string, mixed> {

    try {

      if (!is_object($obj)) {
        throw new InvalidObjectException('passedType='.gettype($obj));
      }

      $reflectedClass = new ReflectionClass($obj);

      $properties = $reflectedClass->getProperties();

      $fields = Map {};

      foreach ($properties as $property) {

        $value = $property->getValue($obj);

        if ($value instanceof TypeInterface ||
            $value instanceof StorableObjectInterface) {
          $fields[$property->getName()] = $value;
        }

      }

      return $fields;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public static function getShortNameForTypeBoxName(
    string $className,
  ): string {

    $pregs = array();

    if (preg_match('/([^\\\\]+Box)$/', $className, $pregs)) {
      return $className;
    } else if (preg_match(
                 '/(StorableMap|StorableVector)\<(.*Box)\>$/',
                 $className,
                 $pregs,
               )) {
      $typeName =
        $pregs[1].'<'.self::getShortNameForTypeBoxName($pregs[2]).'>';
      return $typeName;
    }

    return $className;

  }

  public static function getFieldsAndTypesForObject(
    mixed $obj,
  ): Map<string, string> {

    try {

      if (!is_object($obj)) {
        throw new InvalidObjectException('passedType='.gettype($obj));
      }

      $reflectedClass = new ReflectionClass($obj);

      $properties = $reflectedClass->getProperties();

      $fieldAndTypes = Map {};

      foreach ($properties as $property) {

        $value = $property->getValue($obj);

        if (!$value instanceof TypeInterface &&
            !$value instanceof StorableObjectInterface) {
          continue;
        }

        $fieldName = $property->getName();

        $typeText = $property->getTypeText();

        $typeName = self::getShortNameForTypeBoxName($typeText);

        if ($typeName !== null) {
          $fieldAndTypes[$fieldName] = $typeName;
        }

      }

      return $fieldAndTypes;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public static function getIsRequiredAndIsDefaultValue(
    mixed $field,
  ): (bool, bool) {

    if (!is_object($field)) {
      throw new InvalidObjectException('passedType='.gettype($field));
    }

    if ($field instanceof TypeInterface) {
      $isRequired = $field->getIsRequired();
      list($isDefaultValue, $reason) = $field->isDefaultValue();
      return tuple($isRequired, $isDefaultValue);
    } else if ($field instanceof StorableObjectInterface) {
      $isRequired = $field->getIsRequired();
      list($isDefaultValue, $fields) = $field->isDefaultValue();
      return tuple($isRequired, $isDefaultValue);
    }

    throw new UnsupportedTypeException(get_class($field));

  }

}
