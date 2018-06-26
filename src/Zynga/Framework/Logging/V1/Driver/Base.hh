<?hh // strict 

namespace Zynga\Framework\Logging\V1\Driver;

use Zynga\Framework\Logging\V1\Interfaces\LoggerInterface;
use Zynga\Framework\Logging\V1\Interfaces\LoggerConfigInterface;
use Zynga\Framework\Logging\V1\Interfaces\LoggerAdapterInterface;
use Zynga\Framework\Logging\V1\Level;

use \Exception;

abstract class Base implements LoggerInterface, LoggerAdapterInterface {
  private LoggerConfigInterface $_config;
  private bool $_hideAllLogs;

  public function __construct(LoggerConfigInterface $config) {
    $this->_config = $config;
    $this->_hideAllLogs = false;
  }

  public function getConfig(): LoggerConfigInterface {
    return $this->_config;
  }

  public function setHideAllLogs(bool $logState): bool {
    $this->_hideAllLogs = $logState;
    return true;
  }

  public function exception(string $message, Map<string, mixed> $data, Exception $exception, bool $includeBacktrace = true): bool {
   
    // Capture exception related information
    $data['exceptionClass']         = get_class($exception);
    $data['exceptionMessage']       = $exception->getMessage();
    $data['exceptionBacktrace']     = $exception->getTraceAsString();
    
    return $this->_recordLogEntry(
      Level::ERROR,
      $message,
      $data,
      $includeBacktrace
    );

  }

  public function critical(string $message, Map<string, mixed> $data, bool $includeBacktrace = true): bool {
    return $this->_recordLogEntry(
      Level::CRITICAL,
      $message,
      $data,
      $includeBacktrace
    );
  }

  public function error(string $message, Map<string, mixed> $data, bool $includeBacktrace = true): bool {
    return $this->_recordLogEntry(
      Level::ERROR,
      $message,
      $data,
      $includeBacktrace
    );
  }

  public function warning(string $message, Map<string, mixed> $data, bool $includeBacktrace = false): bool {
    return $this->_recordLogEntry(
      Level::WARNING,
      $message,
      $data,
      $includeBacktrace
    );
    return true;
  }

  public function info(string $message, Map<string, mixed> $data, bool $includeBacktrace = false): bool {
    return $this->_recordLogEntry(
      Level::INFO,
      $message,
      $data,
      $includeBacktrace
    );
  }
  
  public function debug(string $message, Map<string, mixed> $data, bool $includeBacktrace = false): bool {
    return $this->_recordLogEntry(
      Level::DEBUG,
      $message,
      $data,
      $includeBacktrace
    );
  }

  public function formatBacktrace(mixed $bt): string {
    $logString = '';
    if ( ! is_array($bt) ) {
      return $logString;
    }
    foreach($bt as $number => $data) {
      if (array_key_exists('file', $data) && array_key_exists('line', $data)) {
        $file = $data['file'];
        $line = $data['line'];
        $fileName = substr($file, strrpos($file, '/', -1)+1, strlen($file));
        $logString = $logString.$fileName.'@'.$line."-";
      }
    }
    return $logString;
  }

  private function _recordLogEntry(int $level, string $message, Map<string, mixed> $data, bool $includeBacktrace): bool {

    if ( $this->_hideAllLogs === true ) {
      return false;
    }

    if ( $this->_config->shouldLog($level) !== true ) {
      return false;
    }

    if ( $includeBacktrace === true && $data->containsKey('backtrace') !== true ) {
      $data['backtrace'] = $this->formatBacktrace(debug_backtrace());
    }

    return $this->writeLogEntry($level, $message, $data);

  }

  abstract public function writeLogEntry(int $level, string $message, Map<string, mixed> $data): bool;

}
