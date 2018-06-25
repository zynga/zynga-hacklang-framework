<?hh // strict 

namespace Zynga\Framework\Logging\V1;

class StaticLogger {

  public static function setHideAllLogs(bool $logState, string $context = 'default'): bool {
    return true;
  }

  public static function exception(string $message, Map<string, mixed> $data, Exception $exception, bool $includeBacktrace = true, string $context = 'default'): bool {
    return true;
  }

  public static function critical(string $message, Map<string, mixed> $data, bool $includeBacktrace = true, string $context = 'default'): bool {
    return true;
  }

  public static function error(string $message, Map<string, mixed> $data, bool $includeBacktrace = true, string $context = 'default'): bool {
    return true;
  }

  public static function warning(string $message, Map<string, mixed> $data, bool $includeBacktrace = false, string $context = 'default'): bool {
    return true;
  }

  public static function info(string $message, Map<string, mixed> $data, bool $includeBacktrace = false, string $context = 'default'): bool {
    return true;
  }
  
  public static function debug(string $message, Map<string, mixed> $data, bool $includeBacktrace = false, string $context = 'default'): bool {
    return true;
  }

}
