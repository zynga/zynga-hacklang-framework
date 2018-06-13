<?hh // strict

namespace Zynga\Framework\Service\V1\Handler;

use Zynga\Framework\Service\V1\Interfaces\HandlerInterface;
use Zynga\Framework\Service\V1\Interfaces\ServiceInterface;

use Zynga\Framework\Type\V1\StringBox;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Legacy\Log\V1\Log;

abstract class Base implements HandlerInterface {
  private ?ServiceInterface $_service;

  public function __construct() {}

  public function setService(ServiceInterface $service): bool {
    $this->_service = $service;
    return true;
  }

  public function getService(): ?ServiceInterface {
    return $this->_service;
  }

  public function handleGenericFailure(): bool {
    if ($this->_service instanceof ServiceInterface) {
      $this->_service->response()->success()->set(false);
      return true;
    }
    return false;
  }

  public function handleGenericSuccess(): bool {

    $service = $this->_service;

    if (!$service instanceof ServiceInterface || $service === null) {
      return false;
    }

    $response = $service->response();

    $response->success()->set(true);

    return true;

  }

  public function handleFailureException(Exception $e): bool {

    $service = $this->_service;

    if (!$service instanceof ServiceInterface) {
      return false;
    }

    // let the delegates have a chance to deal with generic failure state.
    $this->handleGenericFailure();

    // --
    // JEO: we strip all the pathing out other than the shortname of the exception.
    // --
    $className = $this->createShortExceptionName($e);

    $failureMessage = $className.': '.$e->getMessage();

    $service->response()->addMessage($failureMessage);
    $message = $service->response()->message()->export()->asJSON();

    Log::error_log(
      'Service Handler failed',
      array(
        'failureMessage' => $failureMessage,
        'service' => get_class($service),
        'method' => __METHOD__,
        'stacktrace' => $e->getTraceAsString(),
        'request' => $service->request(),
      ),
    );

    return true;

  }

  public function createShortExceptionName(Exception $e): string {
    $className = get_class($e);
    $className = preg_replace('/.*\\\\Exceptions\\\\/', '', $className);
    return $className;
  }

}
