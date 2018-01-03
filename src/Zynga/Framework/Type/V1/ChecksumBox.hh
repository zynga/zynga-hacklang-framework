<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;

class ChecksumBox extends StringBox {
  <<__Override>>
  protected function importFromString(string $value): bool {
    if (preg_match('/^[a-fA-F0-9]{1,1024}$/', $value)) {
      return parent::importFromString($value);
    }

    throw new FailedToImportFromStringException(
      'invalid checksum value='.$value,
    );
  }
}
