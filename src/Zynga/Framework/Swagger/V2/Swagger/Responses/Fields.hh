<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger\Responses;

use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Exception\V1\Exception;

class Fields implements FieldsInterface {

  public function getForObject(): Map<string, mixed> {
    $map = Map {};
    return $map;
  }

  public function getFieldsAndTypesForObject(): Map<string, string> {
    $map = Map {};
    return $map;
  }

  public function getRequiredFields(): Vector<string> {
    $vec = Vector {};
    return $vec;
  }

  public function getRequiredFieldsWithDefaultValues(
    string $context = '',
  ): Vector<string> {
    $vec = Vector {};
    return $vec;
  }

  public function getTypedField(string $fieldName): TypeInterface {
    throw new Exception('broken-get-typed-field');
  }

}
