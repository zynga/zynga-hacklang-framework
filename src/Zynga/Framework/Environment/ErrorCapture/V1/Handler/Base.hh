<?hh // strict

namespace Zynga\Framework\Environment\ErrorCapture\V1\Handler;

use
  Zynga\Framework\Environment\ErrorCapture\V1\Interfaces\ErrorCaptureInterface
;

abstract class Base implements ErrorCaptureInterface {

  abstract public function handle(
    int $err_no,
    string $err_str,
    string $err_file,
    int $err_line,
  ): void;

  public function start(): void {
    set_error_handler(array($this, 'handle'));
  }

  public function stop(): void {
    restore_error_handler();
  }

}
