<?hh // strict

namespace Zynga\Framework\CommandLine\V1\Interfaces;

interface ArgumentInterface {
  public function getName(): string;
  public function getDescription(): string;
  public function isRequired(): bool;
  public function set(mixed $value): bool;
  public function get(): mixed;
  public function setHasValue(bool $val): bool;
  public function hasValue(): bool;
  public function parseArgument(string $argument): bool;
  public function getArgumentRegex(): string;
  public function getArgumentUsage(): string;
}
