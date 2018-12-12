<?hh // strict

namespace Zynga\Framework\Service\V2\Test;

use Zynga\Framework\Service\V2\Base;
use Zynga\Framework\Service\V2\Test\Request;
use Zynga\Framework\Service\V2\Test\Response;
use Zynga\Framework\Service\V2\Test\Server;

use Zynga\Environment\V1\Interfaces\HTTP\HeaderContainerInterface;
use Zynga\Environment\V1\Test\HTTP\MockHeaderContainer;

use Zynga\Framework\Exception\V1\Exception;

class BrokenHandle extends Base {

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
    throw new Exception('OH-NOZ');
  }
}
