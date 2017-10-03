<?hh // strict

namespace Zynga\Framework\Type\V1\Interfaces;

interface TypeInterface {

  public function reset(): bool;

  public function getIsRequired(): bool;
  public function setIsRequired(bool $isRequired): bool;

  public function isDefaultValue(): (bool, Vector<string>);
  public function setIsDefaultValue(bool $tf): bool;

  public function setDefaultValue(mixed $value): bool;

  public function get(): mixed;
  public function set(mixed $value): bool;

  // These are internal functions utilized within the outward interfaces, please
  // do not expose these as it would mess up functionality like required.
  /*
   public function import(mixed $value): bool;
   public function importFromString(string $value): bool;
   public function importFromInteger(int $value): bool;
   public function importFromFloat(float $value): bool;
   public function importFromBool(bool $value): bool;
   */

}
