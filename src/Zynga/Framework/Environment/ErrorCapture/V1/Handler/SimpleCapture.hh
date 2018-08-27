<?hh // strict

namespace Zynga\Framework\Environment\ErrorCapture\V1\Handler;

use Zynga\Framework\Environment\ErrorCapture\V1\Handler\Base;

class SimpleCapture extends Base {
  private Vector<string> $_errorMessages = Vector {};

  public function handle(
    int $err_no,
    string $err_str,
    string $err_file,
    int $err_line,
  ): void {
    file_put_contents('/tmp/jeo-test.txt', $err_str);
    $this->_errorMessages->add($err_str);
  }

  public function getErrorMessages(): Vector<string> {
    return $this->_errorMessages;
  }

}
