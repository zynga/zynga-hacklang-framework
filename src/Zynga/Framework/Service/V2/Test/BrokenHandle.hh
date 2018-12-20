<?hh // strict

namespace Zynga\Framework\Service\V2\Test;

use Zynga\Framework\Service\V2\Base;
use Zynga\Framework\Service\V2\Test\Request;
use Zynga\Framework\Service\V2\Test\Response;
use Zynga\Framework\Service\V2\Test\ServerGlobals;

use Zynga\Environment\V1\Interfaces\HTTP\HeaderContainerInterface;
use Zynga\Environment\V1\Test\HTTP\MockHeaderContainer;

use Zynga\Framework\Exception\V1\Exception;

class BrokenHandle extends Base {

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
    throw new Exception('OH-NOZ');
  }
}
