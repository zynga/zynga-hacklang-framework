<?hh // strict

namespace Zynga\Framework\Environment\HTTP\HeaderContainer\V2\Test;

use Zynga\Framework\Environment\HTTP\HeaderContainer\V2\Interfaces\HeaderContainerInterface;

class MockHeaderContainer implements HeaderContainerInterface {
  private Map<string, string> $_headers;

  public function __construct() {
    $this->_headers = Map {};
  }

  public function getHeader(string $header): string {
    return $this->_headers[$header];
  }

  public function setHeader(string $header, string $value): bool {
    $this->_headers[$header] = $value;
    return true;
  }

  public function noCaching(): bool {
    return true;
  }

  public function contentIsJSON(): bool {
    return true;
  }

  public function send(): bool {
    return true;
  }

}
