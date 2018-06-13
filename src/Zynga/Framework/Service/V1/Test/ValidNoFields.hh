<?hh // strict

namespace Zynga\Framework\Service\V1\Test;

use Zynga\Framework\Service\V1\Base;
use Zynga\Framework\Service\V1\Test\RequestNoFields;
use Zynga\Framework\Service\V1\Test\Response;

use Zynga\Environment\V1\Interfaces\HTTP\HeaderContainerInterface;
use Zynga\Environment\V1\Test\HTTP\MockHeaderContainer;

class ValidNoFields extends Base {

  private ?RequestNoFields $_request;
  private ?Response $_response;

  public function request(): RequestNoFields {
    if ($this->_request === null) {
      $this->_request = new RequestNoFields();
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
