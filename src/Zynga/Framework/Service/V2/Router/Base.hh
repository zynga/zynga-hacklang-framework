<?hh // strict

namespace Zynga\Framework\Service\V2\Router;

use Zynga\Framework\Service\V2\Base as ServiceBase;

use Zynga\Framework\Dynamic\V1\DynamicClassCreation;

use Zynga\Framework\Service\V2\Interfaces\ServiceInterface;
use Zynga\Framework\Service\V2\Router\Request;
use Zynga\Framework\Service\V2\Router\Response;

use Zynga\Framework\Service\V2\Interfaces\ResponseInterface;

use Zynga\Framework\Type\V1\StringBox;

use Zynga\Framework\Exception\V1\Exception;

use Zynga\Framework\Service\V2\Service\Group\Config\Base as ConfigBase;

abstract class Base extends ServiceBase {
  private ConfigBase $_config;
  private ?Request $_request;
  private ?ResponseInterface $_response;

  public function __construct(ConfigBase $config) {

    parent::__construct();

    $this->_config = $config;

  }

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

  public function handle(): bool {
    // --
    // Okay given the path that was reported, can we make a solid class path?
    // --
    $jobPath = $this->request()->servicePath->get();

    // strip leading or trailing slashes
    $shortName = rtrim($jobPath, '/');
    $shortName = ltrim($shortName, '/');
    $shortName = str_replace('/', '\\', $shortName);

    $className = "";
    foreach ( $this->_config->patterns as $pattern ) {
       $candidateClassName = $pattern->codePath->get().'\\'.$shortName;
       if ( class_exists($candidateClassName) ) {
         $className = $candidateClassName;
       }
    }

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
