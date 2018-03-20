<?hh // strict

namespace Zynga\Framework\IO\Web\V1\Mock;

use Zynga\Framework\Type\V1\UrlBox;
use Zynga\Framework\IO\Web\V1\Manager as BaseManager;

class ManagerWithUnexpectedHttpCode extends BaseManager{

  <<__Override>>
  protected static function getCurlInfo(mixed $curlHandle, int $flag): mixed {
    return false;
  }

  <<__Override>>
  protected static function executeCurl(mixed $curlHandle): mixed {
    return true;
  }

}
