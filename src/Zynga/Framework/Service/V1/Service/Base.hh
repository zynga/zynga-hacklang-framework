<?hh // strict

namespace Zynga\Framework\Service\V1\Service;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Service\V1\Interfaces\ServiceInterface;
use Zynga\Framework\Service\V1\Interfaces\HandlerInterface;
use Zynga\Framework\Service\V1\Handler\Noop as NoopHandler;
use Zynga\Framework\Service\V1\Handler\Http as HttpHandler;
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;

abstract class Base implements ServiceInterface {
  const string NOOP_HANDLER = 'NOOP';
  const string HTTP_HANDLER = 'HTTP';

  private HandlerInterface $_handler;

  public function __construct() {
    $this->_handler = new NoopHandler();
  }

  public function getHandler(): HandlerInterface {
    return $this->_handler;
  }

  public function setHandler(HandlerInterface $handler): bool {
    $this->_handler = $handler;
    return true;
  }

  public function setHandlerFromString(string $method): bool {
    if ($method === self::HTTP_HANDLER) {
      $this->_handler = new HttpHandler();
      return true;
    } else if ($method === self::NOOP_HANDLER) {
      $this->_handler = new NoopHandler();
      return true;
    }
    return false;
  }

  public function handleRequest(bool $handleResponse = true): bool {
    $handler = $this->getHandler();

    try {

      // register this service with the handler
      $handler->setService($this);

      // delegate down the parsing of the request
      $handler->parseRequest();

      // --
      // Run the microservice.
      // --
      if ($this->handle() === true) {
        $handler->handleGenericSuccess();
      } else {
        $handler->handleGenericFailure();
      }

    } catch (Exception $e) {
      $handler->handleFailureException($e);
    }

    if ($handleResponse === false) {
      return $this->response()->success()->get();
    }

    // --
    // Handle the Response message.
    // --
    return $handler->handleResponse();

  }

}
