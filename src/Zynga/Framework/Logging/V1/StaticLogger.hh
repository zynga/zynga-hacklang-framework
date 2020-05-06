<?hh // strict

namespace Zynga\Framework\Logging\V1;

use Zynga\Framework\Logging\V1\Factory as LogFactory;
use Zynga\Framework\Logging\V1\Interfaces\LoggerInterface;
use \Exception;

class StaticLogger {

  public static function setHideAllLogs(bool $logState, string $context = 'default'): bool {
    $logger = LogFactory::factory(LoggerInterface::class, $context);
    return $logger->setHideAllLogs($logState);
  }

  public static function exception(string $message, Map<string, mixed> $data, Exception $exception, bool $includeBacktrace = true, string $context = 'default', float $sampleRate = 100.0, ?int $valueToSampleBy = null): bool {
    $logger = LogFactory::factory(LoggerInterface::class, $context);
    return $logger->exception($message, $data, $exception, $includeBacktrace, $sampleRate, $valueToSampleBy);
  }

  public static function critical(string $message, Map<string, mixed> $data, bool $includeBacktrace = true, string $context = 'default'): bool {
    $logger = LogFactory::factory(LoggerInterface::class, $context);
    return $logger->critical($message, $data, $includeBacktrace);
  }

  public static function error(string $message, Map<string, mixed> $data, bool $includeBacktrace = true, string $context = 'default', float $sampleRate = 100.0, ?int $valueToSampleBy = null): bool {
    $logger = LogFactory::factory(LoggerInterface::class, $context);
    return $logger->error($message, $data, $includeBacktrace, $sampleRate, $valueToSampleBy);
  }

  public static function warning(string $message, Map<string, mixed> $data, bool $includeBacktrace = false, string $context = 'default'): bool {
    $logger = LogFactory::factory(LoggerInterface::class, $context);
    return $logger->warning($message, $data, $includeBacktrace);
  }

  public static function info(string $message, Map<string, mixed> $data, bool $includeBacktrace = false, string $context = 'default'): bool {
    $logger = LogFactory::factory(LoggerInterface::class, $context);
    return $logger->info($message, $data, $includeBacktrace);
  }

  public static function debug(string $message, Map<string, mixed> $data, bool $includeBacktrace = false, string $context = 'default'): bool {
    $logger = LogFactory::factory(LoggerInterface::class, $context);
    return $logger->debug($message, $data, $includeBacktrace);
  }

}
