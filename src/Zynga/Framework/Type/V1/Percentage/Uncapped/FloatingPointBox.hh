<?hh //strict

namespace Zynga\Framework\Type\V1\Percentage\Uncapped;

use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromFloatException;
use Zynga\Framework\Type\V1\FloatBox;

/**
 * Representaion of a floating point percentage
 */
class FloatingPointBox extends FloatBox {
  <<__Override>>
  protected function importFromFloat(float $value): bool {
    if ($value < 0.0) {
      throw new FailedToImportFromFloatException('invalid value='.$value);
    }

    return parent::importFromFloat($value);
  }
}
