<?hh // strict

namespace Zynga\Framework\Service\V2\Test;

use Zynga\Framework\Service\V2\Base;
use Zynga\Framework\Service\V2\Test\RequiredRequest;
use Zynga\Framework\Service\V2\Test\RequiredResponse;
use Zynga\Framework\Service\V2\Test\ServerGlobals;

use Zynga\Environment\V1\Interfaces\HTTP\HeaderContainerInterface;
use Zynga\Environment\V1\Test\HTTP\MockHeaderContainer;

class ValidRequired extends Base {

  private ?RequiredRequest $_request;
  private ?RequiredResponse $_response;
  private ?ServerGlobals $_serverGlobals;

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

  public function serverGlobals(): ServerGlobals {
    if ($this->_serverGlobals === null) {
      $this->_serverGlobals = new ServerGlobals();
    }
    return $this->_serverGlobals;
  }

  public function handle(): bool {
    return true;
  }

}
