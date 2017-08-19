<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Type\V1\StringBox;

use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromBoolException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromFloatException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromIntegerException;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;

/**
 * Given a string, this will ensure that it is a valid email format.
 * Example email:
 * foo@somewhere.com
 */
class EmailBox extends StringBox {

  protected function importFromBool(bool $value): bool {
    throw new FailedToImportFromBoolException();
  }

  protected function importFromFloat(float $value): bool {
    throw new FailedToImportFromFloatException();
  }

  protected function importFromInteger(int $value): bool {
    throw new FailedToImportFromIntegerException();
  }

  protected function importFromString(string $value): bool {
    parent::importFromString($value);

    $matches = array();

    //From RFC 2822 - http://tools.ietf.org/html/rfc2822#section-3.4.1
    $numMatches =
      preg_match_all(
        '/^(?:[a-z0-9!#$%&\'*+\/=?^_`{|}~-]+(?:\\.[a-z0-9!#$%&\'*+\/=?^_`{|}~-]+)*|"(?:[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21\\x23-\\x5b\\x5d-\\x7f]|\\\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])*")'.
        '@'.
        '(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\\.)+'.
        '[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\\[(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}'.
        '(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?|[a-z0-9-]*[a-z0-9]:(?:[\\x01-\\x08\\x0b\\x0c\\x0e-\\x1f\\x21-\\x5a\\x53-\\x7f]|\\\\[\\x01-\\x09\\x0b\\x0c\\x0e-\\x7f])+'.
        ')\\])/i',
        $this->_value,
        $matches,
      );

    if ($numMatches !== 1) {
      throw new FailedToImportFromStringException(
        'Unable to import string. value='.$value,
      );
    }

    return true;
  }

}
