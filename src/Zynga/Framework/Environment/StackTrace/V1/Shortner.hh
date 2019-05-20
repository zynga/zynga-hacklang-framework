<?hh // strict

namespace Zynga\Framework\Environment\Stacktrace\V1;

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
  ): void {
    if (array_key_exists($elem, $frame)) {
      $this->addFragment($elem.'='.strval($frame[$elem]));
    }
  }

}

class Shortner {

  public static function toString(
    int $trimFrames = 0,
    string $delim = "|",
  ): string {

    $frames = debug_backtrace();

    if ($trimFrames > 0) {
      for ($i = 0; $i < $trimFrames; $i++) {
        array_pop($frames);
      }
    }

    $buffer = '';

    foreach ($frames as $frame) {

      $frameBuffer = new FrameBuffer();
      //$frameBuffer->addFromFrame($frame, 'file');
      $frameBuffer->addFromFrame($frame, 'class');
      $frameBuffer->addFromFrame($frame, 'function');
      $frameBuffer->addFromFrame($frame, 'line');

      if ($buffer != '') {
        $buffer .= $delim;
      }

      $buffer .= $frameBuffer->get();

    }

    return $buffer;

  }

}
