<?hh // strict

namespace Zynga\Framework\Service\V1\Interfaces\Authenticated;

use
  Zynga\Framework\Service\V1\Interfaces\ServiceInterface as BaseServiceInterface
;

interface ServiceInterface extends BaseServiceInterface {
  public function authenticatedHandle(): bool;
}
