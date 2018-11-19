<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromIntegerException;
use Zynga\Framework\Type\V1\Int32Box;

/**
 * Encapsulation of a UTC offset in hours
 */
class UtcOffsetBox extends Int32Box {
  <<__Override>>
  protected function importFromInteger(int $value): bool {
    if ($this->isValidUtcOffset($value)) {
      parent::importFromInteger($value);
      return true;
    }

    throw new FailedToImportFromIntegerException('value='.$value);
  }

  /**
   * Determines if the given UTC offset is valid
   *
   * @param $utcOffset Integer representation of a UTC offset in hours
   * @return True if the given UTC offset is valid, otherwise false
   */
  private function isValidUtcOffset(int $utcOffset): bool {
    return $utcOffset >= -12 && $utcOffset <= 14;
  }
}
