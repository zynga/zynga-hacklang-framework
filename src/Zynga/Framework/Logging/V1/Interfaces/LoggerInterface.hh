<?hh // strict

namespace Zynga\Framework\Logging\V1\Interfaces;

use \Exception;
use Zynga\Framework\Factory\V2\Interfaces\DriverInterface as BaseDriverInterface;
use Zynga\Framework\Logging\V1\Interfaces\LoggerConfigInterface;

interface LoggerInterface extends BaseDriverInterface {

  public function getConfig(): LoggerConfigInterface;
  public function setHideAllLogs(bool $logState): bool;
  public function exception(string $message, Map<string, mixed> $data, Exception $exception, bool $includeBacktrace = true, float $sampleRate = 100.0, ?int $valueToSampleBy = null): bool;
  public function critical(string $message, Map<string, mixed> $data, bool $includeBacktrace = true): bool;
  public function error(string $message, Map<string, mixed> $data, bool $includeBacktrace = true, float $sampleRate = 100.0, ?int $valueToSampleBy = null): bool;
  public function warning(string $message, Map<string, mixed> $data, bool $includeBacktrace = false): bool;
  public function info(string $message, Map<string, mixed> $data, bool $includeBacktrace = false): bool;
  public function debug(string $message, Map<string, mixed> $data, bool $includeBacktrace = false): bool;

}
