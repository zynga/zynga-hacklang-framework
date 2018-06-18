<?hh // strict

namespace Zynga\Framework\Service\V2\Test;

use Zynga\Framework\Service\V2\Base;
use Zynga\Framework\Service\V2\Test\Request;
use Zynga\Framework\Service\V2\Test\Response;

use Zynga\Environment\V1\Interfaces\HTTP\HeaderContainerInterface;
use Zynga\Environment\V1\Test\HTTP\MockHeaderContainer;

class Valid extends Base {

  private ?Request $_request;
  private ?Response $_response;

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

  public function handle(): bool {
    return true;
  }

}
