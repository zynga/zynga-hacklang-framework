<?hh // strict

namespace Zynga\Framework\Service\V2\Test\Router;

use Zynga\Framework\Service\V2\Router\Base as ServiceRouterBase;

class Mock extends ServiceRouterBase {

  public function getClassBase(): string {
    return 'Zynga\Framework\Service\V2\Test\Router\Services';
  }

}
