<?hh //strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\Exceptions\StringTooLongException;

/**
 * This box will accept any string of any length unless a max length is specified.
 * Setting the limit to anything less than 0 does not cap the string length.
 */
class FixedLengthStringBox extends StringBox {
  const NO_LIMIT = -1;

  private int $_maxLength;

  public function __construct(int $maxLength = static::NO_LIMIT) {
    $this->_maxLength = $maxLength;

    parent::__construct();
  }

  public function getMaxLength(): int {
    return $this->_maxLength;
  }

  <<__Override>>
  protected function importFromString(string $value): bool {
    $this->_validateString($value);

    return parent::importFromString($value);
  }

  protected function _validateString(string $string): void {
    if ($this->_maxLength <= static::NO_LIMIT) {
      return;
    }

    if (mb_strlen($string) > $this->_maxLength) {
      throw new StringTooLongException(
        'String exceeds limit. string='.
        $string.
        ' maxLength='.
        $this->_maxLength,
      );
    }
  }
}
