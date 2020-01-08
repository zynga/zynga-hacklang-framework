<?hh // strict

namespace Zynga\Framework\Environment\ErrorCapture\V1\Exceptions;

use Zynga\Framework\Exception\V1\Exception as ZyngaException;
use \Exception;

class ErrorException extends ZyngaException {

  public function __construct(
    string $message,
    int $code = 0,
    ?Exception $previous = null,
    ?string $file = null,
    ?int $line = $line,
  ) {

    parent::__construct($message, $code);

    if (is_string($message)) {
      $this->message = $message;
    }
    $this->code = $code;
    if ($previous instanceof Exception) {
      $this->previous = $previous;
    }
    if (is_string($file)) {
      $this->file = $file;
    }
    if (is_int($line)) {
      $this->line = $line;
    }

  }

}
