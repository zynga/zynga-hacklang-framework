<?hh // strict

namespace Zynga\Framework\Testing\CodeCoverage\V1;

use \Exception;

class MultiLineThrow {
  public function doBigThrow(): void {
    throw new Exception(
      'i am a important long message that causes wordwrap to happen'
    );
  }
}
