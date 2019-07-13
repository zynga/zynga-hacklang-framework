<?hh // strict

namespace Zynga\Framework\Environment\ErrorCapture\V1\Handler;

use Zynga\Framework\Environment\ErrorCapture\V1\Handler\Base;
use \Exception;

class ConvertErrorsToExceptions extends Base {

  public function handle(
    int $err_no,
    string $err_str,
    string $err_file,
    int $err_line,
  ): void {
    throw new Exception($err_str, $err_no);
  }

}
