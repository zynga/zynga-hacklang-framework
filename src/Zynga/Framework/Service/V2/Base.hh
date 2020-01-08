<?hh // strict

namespace Zynga\Framework\Service\V2;

use Zynga\Framework\Service\V2\Interfaces\RequestInterface;
use Zynga\Framework\Service\V2\Interfaces\ResponseInterface;
use Zynga\Framework\Service\V2\Interfaces\ServerGlobalsInterface;
use Zynga\Framework\Service\V2\Service\Base as ServiceBase;

abstract class Base extends ServiceBase {

  abstract public function request(): RequestInterface;

  abstract public function handle(): bool;

  abstract public function response(): ResponseInterface;

  abstract public function serverGlobals(): ServerGlobalsInterface;

}
