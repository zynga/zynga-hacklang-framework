<?hh // strict

namespace Zynga\Framework\Environment\ErrorCapture\V1\Handler;

use Zynga\Framework\Environment\ErrorCapture\V1\Exceptions\ErrorException;
use Zynga\Framework\Environment\ErrorCapture\V1\Handler\Base;

class ConvertErrorsToExceptions extends Base {

  public function handle(
    int $err_no,
    string $err_str,
    string $err_file,
    int $err_line,
  ): void {
    throw new ErrorException(
      $err_str.' file='.$err_file.':'.$err_line,
      $err_no,
      null,
      $err_file,
      $err_line,
    );
  }

}
