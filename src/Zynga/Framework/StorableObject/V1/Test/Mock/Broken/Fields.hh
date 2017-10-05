<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Test\Mock\Broken;

use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Exception\V1\Exception;

class Fields implements FieldsInterface {

  public function __construct(StorableObjectInterface $object) {}

  public function getTypedField(string $fieldName): TypeInterface {
    throw new Exception('broken-get-typed-field');
  }

  public function getForObject(): Map<string, mixed> {
    throw new Exception('broken-get-for-object');
  }

  public function getFieldsAndTypesForObject(): Map<string, string> {
    throw new Exception('broken-get-fields-and-types-for-object');
  }

  public function getRequiredFieldsWithDefaultValues(
    string $context = '',
  ): Vector<string> {
    throw new Exception('broken-get-required-fields-with-default-values');
  }

}
