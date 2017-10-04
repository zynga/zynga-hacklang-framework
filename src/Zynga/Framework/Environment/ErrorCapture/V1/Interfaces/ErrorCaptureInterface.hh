<?hh // strict

namespace Zynga\Framework\Environment\ErrorCapture\V1\Interfaces;

interface ErrorCaptureInterface {
  public function handle(
    int $err_no,
    string $err_str,
    string $err_file,
    int $err_line,
  ): void;
  public function start(): void;
  public function stop(): void;
}
