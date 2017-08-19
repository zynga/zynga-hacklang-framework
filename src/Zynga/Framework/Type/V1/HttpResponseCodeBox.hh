<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Type\V1\UInt32Box;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromIntegerException;

class HttpResponseCodeBox extends UInt32Box {
  const int HTTP_OK = 200;
  const int HTTP_FAILURE = 400;
  const int HTTP_AUTH_FAILURE = 403;

  <<__Override>>
  protected function importFromInteger(int $value): bool {
    try {
      if ($value == self::HTTP_OK ||
          $value == self::HTTP_FAILURE ||
          $value == self::HTTP_AUTH_FAILURE) {
        return parent::importFromInteger($value);
      }
      throw new FailedToImportFromIntegerException(
        'unsupported http_code='.$value,
      );
    } catch (Exception $e) {
      throw $e;
    }
  }

  public static function ok(): HttpResponseCodeBox {
    $resp = new HttpResponseCodeBox();
    $resp->set(HttpResponseCodeBox::HTTP_OK);

    return $resp;
  }
}
