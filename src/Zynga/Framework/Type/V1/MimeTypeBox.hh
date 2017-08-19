<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException;

class MimeTypeBox extends StringBox {
  const string JSON = 'application/json';

  <<__Override>>
  protected function importFromString(string $value): bool {
    try {
      if ($value == self::JSON) {
        return parent::importFromString($value);
      }
      throw new FailedToImportFromStringException(
        'unsupported mime_type='.$value,
      );
    } catch (Exception $e) {
      throw $e;
    }
  }

}
