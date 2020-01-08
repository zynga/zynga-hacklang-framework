<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Object;

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
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

use Zynga\Framework\Exception\V1\Exception;

class Importer implements ImportInterface {
  protected StorableObjectInterface $_object;

  public function __construct(StorableObjectInterface $object) {
    $this->_object = $object;
  }

  public function fromVector(Vector<mixed> $data): bool {

    try {

      $fields = $this->_object->fields()->getForObject();

      if ($fields->count() == 0) {
        throw new NoFieldsFoundException('class='.get_class($this->_object));
      }

      $fieldCount = $fields->count();
      $dataCount = $data->count();

      if ($fieldCount != $dataCount) {
        $message = '';
        $message .= 'Shape mismatch ';
        $message .= 'fieldCount='.$fieldCount.' ';
        $message .= 'dataCount='.$dataCount.' ';
        $message .= 'data='.json_encode($data);
        throw new ExpectedFieldCountMismatchException($message);
      }

      $offset = 0;
      foreach ($fields as $fieldName => $field) {

        $value = $data[$offset];

        if ($value instanceof TypeInterface ||
            $value instanceof StorableObjectInterface) {
          throw new UnsupportedTypeException('type='.get_class($value));
        }

        if ($field instanceof TypeInterface) {
          $field->set($value);
        }

        $offset++;
      }

      return true;

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function fromMap(Map<string, mixed> $data): bool {
    try {

      $fields = $this->_object->fields()->getForObject();

      if ($fields->count() == 0) {
        throw new NoFieldsFoundException('class='.get_class($this->_object));
      }

      foreach ($fields as $fieldName => $field) {
        if ($data->containsKey($fieldName) !== true) {
          continue;
        }

        $value = $data[$fieldName];

        if ($value instanceof TypeInterface ||
            $value instanceof StorableObjectInterface) {
          throw new UnsupportedTypeException(
            'fieldName='.$fieldName.' type='.get_class($value),
          );
        }

        if ($field instanceof TypeInterface) {
          $field->set($value);
        } else if ($field instanceof StorableObjectInterface) {
          if (is_array($value)) {
            $field->import()->fromMap(new Map($value));
          } else if ($value instanceof Map) {
            $field->import()->fromMap($value);
          } else if (is_string($value)) {
            $field->import()->fromJSON($value);
          }
        }
      }

      $defaultValueRequiredFields =
        $this->_object->fields()->getRequiredFieldsWithDefaultValues();

      if ($defaultValueRequiredFields->count() > 0) {
        $message =
          'Failed to import one or more fields. Make sure that '.
          'the data you\'re importing is of a supported type '.
          '(either string or JSON).';
        $message .= ' fields='.json_encode($defaultValueRequiredFields);
        throw new MissingKeyFromImportDataException($message);
      }

      return true;
    } catch (Exception $e) {
      throw $e;
    }

  }

  public function fromJSON(string $payload): bool {

    try {

      $rawData = json_decode($payload, true);
      $rawMap = new Map($rawData);
      return $this->fromMap($rawMap);

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function fromBinary(string $payload): bool {
    return false;
  }

}
