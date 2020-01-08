<?hh // strict

namespace Zynga\Framework\Environment\HTTP\HeaderContainer\V2\Test\Mock;

use
  Zynga\Framework\Environment\HTTP\HeaderContainer\V2\HeaderContainer as BaseHeaderContainer
;

class HeaderContainer extends BaseHeaderContainer {
  private bool $_doHeadersSent = true;
  private bool $_headersSentRv = false;

  private bool $_doSend = true;
  private bool $_sendRv = false;

  public function setDoHeadersSent(bool $tf): bool {
    $this->_doHeadersSent = $tf;
    return true;
  }

  public function setHeadersSentRv(bool $tf): bool {
    $this->_headersSentRv = $tf;
    return true;
  }

  public function setDoSend(bool $tf): bool {
    $this->_doSend = $tf;
    return true;
  }

  public function setSendRv(bool $tf): bool {
    $this->_sendRv = $tf;
    return true;
  }

  public function headersSent(): bool {
    if ($this->_doHeadersSent) {
      return parent::headersSent();
    }
    return $this->_headersSentRv;
  }

  public function send(): bool {
    if ($this->_doSend) {
      return parent::send();
    }
    return $this->_sendRv;
  }

}
