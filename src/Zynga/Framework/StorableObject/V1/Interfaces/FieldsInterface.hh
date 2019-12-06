<?hh // strict

namespace Zynga\Framework\StorableObject\V1\Interfaces;

use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

interface FieldsInterface {
  public function getForObject(): Map<string, mixed>;
  public function getFieldsAndTypesForObject(): Map<string, string>;
  public function getRequiredFields(): Vector<string>;
  public function getRequiredFieldsWithDefaultValues(
    string $context = '',
  ): Vector<string>;
  public function getTypedField(string $fieldName): TypeInterface;
}
