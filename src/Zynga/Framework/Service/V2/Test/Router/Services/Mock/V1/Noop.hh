<?hh // strict

namespace Zynga\Framework\Service\V2\Test\Router\Services\Mock\V1;

use Zynga\Framework\Service\V2\Base as ServiceBase;

use Zynga\Framework\Service\V2\Test\Router\Services\Mock\V1\Request;
use Zynga\Framework\Service\V2\Test\Router\Services\Mock\V1\Response;
use Zynga\Framework\Service\V2\Test\Router\Services\Mock\V1\ServerGlobals;

class Noop extends ServiceBase {

  private ?Request $_request;
  private ?Response $_response;
  private ?ServerGlobals $_serverGlobals;

  public function request(): Request {
    if ($this->_request === null) {
      $this->_request = new Request();
    }
    return $this->_request;
  }

  public function response(): Response {
    if ($this->_response === null) {
      $this->_response = new Response();
    }
    return $this->_response;
  }

  public function serverGlobals(): ServerGlobals {
    if ($this->_serverGlobals === null) {
      $this->_serverGlobals = new ServerGlobals();
    }
    return $this->_serverGlobals;
  }

  public function handle(): bool {
    $this->response()->request_class->set(get_class($this));
    return true;
  }

}
