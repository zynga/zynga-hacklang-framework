<?hh // strict

namespace Zynga\Framework\IO\Web\V1\Mock;

use Zynga\Framework\IO\Web\V1\Manager as BaseManager;

class ManagerWithFailedExecuteCurl extends BaseManager{

  <<__Override>>
  protected static function executeCurl(mixed $curlHandle): mixed {
    return false;
  }

}
