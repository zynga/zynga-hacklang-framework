<?hh // strict

namespace Zynga\Framework\Environment\StackTrace\V1;

class FrameBuffer {
  private string $_buffer;

  public function __construct() {
    $this->_buffer = '';
  }

  public function get(): string {
    return $this->_buffer;
  }

  public function addFragment(string $fragment): bool {
    if ($this->_buffer != '') {
      $this->_buffer .= '::';
    }
    $this->_buffer .= $fragment;
    return true;
  }

  public function addFromFrame(
    array<string, mixed> $frame,
    string $elem,
  ): bool {
    if (array_key_exists($elem, $frame)) {
      $this->addFragment($elem.'='.strval($frame[$elem]));
    }
    return true;
  }

}
