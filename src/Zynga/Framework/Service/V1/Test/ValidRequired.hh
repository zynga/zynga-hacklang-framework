<?hh // strict

namespace Zynga\Framework\Service\V1\Test;

use Zynga\Framework\Service\V1\Base;
use Zynga\Framework\Service\V1\Test\RequiredRequest;
use Zynga\Framework\Service\V1\Test\RequiredResponse;

use Zynga\Environment\V1\Interfaces\HTTP\HeaderContainerInterface;
use Zynga\Environment\V1\Test\HTTP\MockHeaderContainer;

class ValidRequired extends Base {

  private ?RequiredRequest $_request;
  private ?RequiredResponse $_response;

  public function request(): RequiredRequest {
    if ($this->_request === null) {
      $this->_request = new RequiredRequest();
    }
    return $this->_request;
  }

  public function response(): RequiredResponse {
    if ($this->_response === null) {
      $this->_response = new RequiredResponse();
    }
    return $this->_response;
  }

  public function handle(): bool {
    return true;
  }

}
