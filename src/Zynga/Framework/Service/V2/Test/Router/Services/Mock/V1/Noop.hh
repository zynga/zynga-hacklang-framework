<?hh // strict

namespace Zynga\Framework\Service\V2\Test\Router\Services\Mock\V1;

use Zynga\Framework\Service\V2\Base as ServiceBase;

use Zynga\Framework\Service\V2\Test\Router\Services\Mock\V1\Request;
use Zynga\Framework\Service\V2\Test\Router\Services\Mock\V1\Response;
use Zynga\Framework\Service\V2\Test\Router\Services\Mock\V1\Server;

class Noop extends ServiceBase {

  private ?Request $_request;
  private ?Response $_response;
  private ?Server $_server;

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

  public function server(): Server {
    if ($this->_server === null) {
      $this->_server = new Server();
    }
    return $this->_server;
  }

  public function handle(): bool {
    $this->response()->request_class->set(get_class($this));
    return true;
  }

}
