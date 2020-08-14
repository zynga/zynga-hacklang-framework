<?hh // strict

namespace Zynga\Framework\Service\V2\Handler;

use Zynga\Framework\Service\V2\Interfaces\HandlerInterface;
use Zynga\Framework\Service\V2\Interfaces\ServiceInterface;

use Zynga\Framework\Type\V1\StringBox;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Logging\V1\StaticLogger;

abstract class Base implements HandlerInterface {
  private ?ServiceInterface $_service;
  private string $_filePath;

  public function __construct() {
    $this->_filePath = '';
  }

  public function getFilePath(): string {
      return $this->_filePath;
  }

  public function setFilePath(string $filePath): bool {
      $this->_filePath = $filePath;
      return true;
  }

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
    
    if($response->success()->isDefaultValue()[0]) {
      $response->success()->set(true);
    }
    
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

    StaticLogger::error('Service Handler failed',
      Map {
        'failureMessage' => $failureMessage,
        'service' => get_class($service),
        'method' => __METHOD__,
        'request' => $service->request(),
        },
      true
    );

    return true;

  }

  public function createShortExceptionName(Exception $e): string {
    $className = get_class($e);
    $className = preg_replace('/.*\\\\Exceptions\\\\/', '', $className);
    return $className;
  }

}
