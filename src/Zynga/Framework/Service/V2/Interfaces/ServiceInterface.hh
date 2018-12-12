<?hh // strict

namespace Zynga\Framework\Service\V2\Interfaces;

use Zynga\Framework\Service\V2\Interfaces\RequestInterface;
use Zynga\Framework\Service\V2\Interfaces\ResponseInterface;
use Zynga\Framework\Service\V2\Interfaces\HandlerInterface;
use Zynga\Framework\Service\V2\Interfaces\ServerInterface;

use Zynga\Framework\Exception\V1\Exception;

interface ServiceInterface {

  public function handleRequest(bool $handleResponse = true): bool;

  public function request(): RequestInterface;

  public function handle(): bool;

  public function response(): ResponseInterface;

  public function server(): ServerInterface;

  public function getHandler(): HandlerInterface;

  public function setHandler(HandlerInterface $handler): bool;

  public function setHandlerFromString(string $method): bool;

}
