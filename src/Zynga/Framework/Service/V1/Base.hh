<?hh // strict

namespace Zynga\Framework\Service\V1;

use Zynga\Framework\Service\V1\Interfaces\RequestInterface;
use Zynga\Framework\Service\V1\Interfaces\ResponseInterface;
use Zynga\Framework\Service\V1\Service\Base as ServiceBase;

abstract class Base extends ServiceBase {

  abstract public function request(): RequestInterface;

  abstract public function handle(): bool;

  abstract public function response(): ResponseInterface;

}
