<?hh // strict

namespace Zynga\Framework\Type\V1\Test\Mock;

use Zynga\Framework\Type\V1\Base;

class BadBox extends Base {
  public function reset(): bool {
    return true;
  }
  protected function importFromInteger(int $value): bool {
    return true;
  }
  protected function importFromString(string $value): bool {
    return true;
  }
  protected function importFromFloat(float $value): bool {
    return true;
  }
  protected function importFromBool(bool $value): bool {
    return true;
  }
  public function get(): mixed {
    return null;
  }
}
