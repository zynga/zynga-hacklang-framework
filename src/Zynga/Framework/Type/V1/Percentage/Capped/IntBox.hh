<?hh //strict

namespace Zynga\Framework\Type\V1\Percentage\Capped;

use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromIntegerException;
use Zynga\Framework\Type\V1\UInt8Box;

/**
 * Representaion of an integer percentage capped at 100.0
 */
class IntBox extends UInt8Box {
  <<__Override>>
  protected function importFromInteger(int $value): bool {
    if ($value > 100) {
      throw new FailedToImportFromIntegerException('invalid value='.$value);
    }

    return parent::importFromInteger($value);
  }
}
