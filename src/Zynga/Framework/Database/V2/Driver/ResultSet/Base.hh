<?hh // strict

namespace Zynga\Framework\Database\V2\Driver\ResultSet;

use Zynga\Framework\Environment\ErrorCapture\V1\Interfaces\ErrorCaptureInterface;
use Zynga\Framework\Environment\ErrorCapture\V1\Handler\Noop as ErrorCaptureNoop;

use Zynga\Framework\Database\V2\Interfaces\ResultSetInterface;

abstract class Base implements ResultSetInterface {
  private ErrorCaptureInterface $_errorCapture;

  public function __construct() {
    $this->_errorCapture = new ErrorCaptureNoop();
  }

  public function errorCapture(): ErrorCaptureInterface {
    return $this->_errorCapture;
  }
}
