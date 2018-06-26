<?hh // strict 

namespace Zynga\Framework\Logging\V1\Adapter;

use Zynga\Framework\Logging\V1\Adapter\Base as AdapterBase;
use Zynga\Framework\Logging\V1\Level;

class ErrorLog extends AdapterBase {

  public function writeLogEntry(int $level, string $message, Map<string, mixed> $data): bool {

    // Pull the timer in, and pretty print it
    $t     = microtime(true);
    $micro = sprintf("%06d",($t - floor($t)) * 1000000);
    $now   = date('Y-m-d\TH:i:s', $t) . '.' . $micro;

    $logEntry = '';

    // timestamp
    $logEntry .= $now;

    // log level aka reason
    $logEntry .= ' log="' . Level::getLevelAsString($level) . '"';

    // log message
    $logEntry .= ' msg="' . $message . '"';

    // user provided data stack
    if ( $data->count() > 0 ) {

      foreach ( $data as $key => $value ) {
        // add a space to the log line
        $logEntry .= ' ';
        // add the key and value, json_encoded
        $logEntry .= $key . '=' . json_encode($value);
      }

    }

    error_log($logEntry);

    return true;

  }

}
