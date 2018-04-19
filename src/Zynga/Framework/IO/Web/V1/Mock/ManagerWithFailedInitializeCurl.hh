<?hh // strict

namespace Zynga\Framework\IO\Web\V1\Mock;

use Zynga\Framework\Type\V1\UrlBox;
use Zynga\Framework\IO\Web\V1\Manager as BaseManager;

class ManagerWithFailedInitializeCurl extends BaseManager {

  <<__Override>>
  protected static function initializeCurl(UrlBox $url): mixed {
    return false;
  }

}
