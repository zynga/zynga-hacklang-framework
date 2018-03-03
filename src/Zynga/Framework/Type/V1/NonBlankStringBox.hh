<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;

/**
 * StringBox which must contain chars other than white space.
 */
class NonBlankStringBox extends StringBox {

  <<__Override>>
  protected function importFromString(string $value): bool {
    if (strlen(trim($value)) == 0) {
      throw new FailedToImportFromStringException(
        'Unexpected blank string found.',
      );
    }

    return parent::importFromString($value);
  }

}
