<?hh // strict

namespace Zynga\Framework\Environment\StackTrace\V1;

use Zynga\Framework\Environment\StackTrace\V1\FrameBuffer;

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
