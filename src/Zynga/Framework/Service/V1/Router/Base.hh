<?hh // strict

namespace Zynga\Framework\Service\V1\Router;

use Zynga\Framework\Service\V1\Base as ServiceBase;

use Zynga\Framework\Dynamic\V1\DynamicClassCreation;

use Zynga\Framework\Service\V1\Interfaces\ServiceInterface;
use Zynga\Framework\Service\V1\Router\Request;
use Zynga\Framework\Service\V1\Router\Response;

use Zynga\Framework\Service\V1\Interfaces\ResponseInterface;

use Zynga\Framework\Type\V1\StringBox;

use Zynga\Framework\Exception\V1\Exception;

abstract class Base extends ServiceBase {

  private ?Request $_request;
  private ?ResponseInterface $_response;

  public function request(): Request {
    if ($this->_request === null) {
      $this->_request = new Request();
    }
    return $this->_request;
  }

  public function response(): ResponseInterface {
    if ($this->_response === null) {
      $this->_response = new Response();
    }
    return $this->_response;
  }

  abstract public function getClassBase(): string;

  public function handle(): bool {
    // --
    // Okay given the path that was reported, can we make a solid class path?
    // --
    $jobPath = $this->request()->servicePath->get();

    // strip leading or trailing slashes
    $shortName = rtrim($jobPath, '/');
    $shortName = ltrim($shortName, '/');
    $shortName = str_replace('/', '\\', $shortName);
    $classBase = $this->getClassBase();

    $className = $classBase.'\\'.$shortName;

    $obj = null;

    try {
      $obj = DynamicClassCreation::createClassByName($className, Vector {});
    } catch (Exception $e) {
      // noop, we know we hit a exception, but the next traps will handle bubble up.
    }

    if ($obj instanceof ServiceInterface) {
      $obj->setHandler($this->getHandler());
      $returnValue = $obj->handleRequest(false);
      $this->_response = $obj->response();
      return $returnValue;
    }

    $failureMessage = new StringBox();
    $failureMessage->set('SERVICE_NOT_FOUND');

    $this->response()->message()->add($failureMessage);

    return false;

  }

}
