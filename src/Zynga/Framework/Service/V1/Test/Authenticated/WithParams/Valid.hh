<?hh // strict

namespace Zynga\Framework\Service\V1\Test\Authenticated\WithParams;

use Zynga\Framework\Service\V1\Authenticated\Base;
use Zynga\Framework\Service\V1\Test\Authenticated\WithParams\Request;
use Zynga\Framework\Service\V1\Test\Authenticated\WithParams\Response;

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

  public function authenticatedHandle(): bool {
    return true;
  }
}
