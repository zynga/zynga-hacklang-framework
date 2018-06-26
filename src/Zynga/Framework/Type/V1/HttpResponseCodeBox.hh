<?hh // strict

namespace Zynga\Framework\Type\V1;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Type\V1\UInt32Box;
use Zynga\Framework\Type\V1\Exceptions\FailedToImportFromIntegerException;

class HttpResponseCodeBox extends UInt32Box {
  const int OK                                = 200;
  const int FAILURE_RANGE_START               = 400;
  const int FAILURE_BAD_REQUEST               = 400;
  const int FAILURE_AUTH                      = 403;
  const int FAILURE_INTERNAL_SERVICE_ERROR    = 500;
  const int FAILURE_RANGE_END                 = 599;

  <<__Override>>
  protected function importFromInteger(int $value): bool {
    try {
      if ($value == self::OK ||
          $value == self::FAILURE_BAD_REQUEST ||
          $value == self::FAILURE_AUTH || 
          $value == self::FAILURE_INTERNAL_SERVICE_ERROR) {
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
    $resp->set(HttpResponseCodeBox::OK);
    return $resp;
  }

}
