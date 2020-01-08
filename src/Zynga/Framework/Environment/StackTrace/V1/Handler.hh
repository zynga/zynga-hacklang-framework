<?hh // strict

namespace Zynga\Framework\Environment\StackTrace\V1;

use Zynga\Framework\Environment\StackTrace\V1\Frame;

class Handler {

  public static function getStackTrace(
    bool $includeSelf = true,
  ): Vector<Frame> {
    $frames = Vector {};

    $options = 2; // we turn off capturing args, and current object
    $limit = 0; // unlimited by default

    $backtrace = debug_backtrace($options, $limit);

    $frameCount = 0;

    foreach ($backtrace as $rawFrame) {

      $frameCount++;

      if ($includeSelf !== true && $frameCount == 1) {
        // skip ingesting this frame
        continue;
      }

      $frame = new Frame();
      $frame->frameId->set($frameCount);

    }

    return $frames;

  }

}
