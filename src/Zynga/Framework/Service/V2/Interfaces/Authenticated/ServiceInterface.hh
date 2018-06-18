<?hh // strict

namespace Zynga\Framework\Service\V2\Interfaces\Authenticated;

use
  Zynga\Framework\Service\V2\Interfaces\ServiceInterface as BaseServiceInterface
;

interface ServiceInterface extends BaseServiceInterface {
  public function authenticatedHandle(): bool;
}
