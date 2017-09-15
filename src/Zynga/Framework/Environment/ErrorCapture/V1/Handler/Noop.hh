<?hh // strict

namespace Zynga\Framework\Environment\ErrorCapture\V1\Handler;

use Zynga\Framework\Environment\ErrorCapture\V1\Handler\Base;

class Noop extends Base {
  public function handle(
    int $err_no,
    string $err_str,
    string $err_file,
    int $err_line,
  ): void {
    // NOOP: this version of our error handler does nothing.
  }
}
