<?hh // strict

namespace Zynga\Framework\StorableObject\Collections\V2\Traits;

use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

/**
 * Trait to enforcement value type for Collections
 * Collection supports value of type StorableObjectInterface or TypeInterface
 */
trait TypeEnforcement {
  private function validateTypeImplementsRequiredInterface<Tv>(
    classname<Tv> $valueType,
  ): void {
    $interfaces = class_implements($valueType);

    if (array_key_exists(StorableObjectInterface::class, $interfaces) === false &&
        array_key_exists(TypeInterface::class, $interfaces) === false) {
      throw new UnsupportedTypeException(
        'Collection only support the following types for Map values:'.
        StorableObjectInterface::class.
        ', '.
        TypeInterface::class,
      );
    }
  }
}
