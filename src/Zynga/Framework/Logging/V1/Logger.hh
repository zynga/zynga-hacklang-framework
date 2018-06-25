<?hh // strict 

namespace Zynga\Framework\Logging\V1;

class Logger {

  public function setHideAllLogs(bool $logState, string $context = 'default'): bool {
    return true;
  }

  public function exception(string $message, Map<string, mixed> $data, Exception $exception, bool $includeBacktrace = true, string $context = 'default'): bool {
    return true;
  }

  public function critical(string $message, Map<string, mixed> $data, bool $includeBacktrace = true, string $context = 'default'): bool {
    return true;
  }

  public function error(string $message, Map<string, mixed> $data, bool $includeBacktrace = true, string $context = 'default'): bool {
    return true;
  }

  public function warning(string $message, Map<string, mixed> $data, bool $includeBacktrace = false, string $context = 'default'): bool {
    return true;
  }

  public function info(string $message, Map<string, mixed> $data, bool $includeBacktrace = false, string $context = 'default'): bool {
    return true;
  }
  
  public function debug(string $message, Map<string, mixed> $data, bool $includeBacktrace = false, string $context = 'default'): bool {
    return true;
  }

}
