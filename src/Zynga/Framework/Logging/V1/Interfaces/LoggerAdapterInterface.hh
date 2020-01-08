<?hh // strict

namespace Zynga\Framework\Logging\V1\Interfaces;

interface LoggerAdapterInterface {
  public function writeLogEntry(int $level, string $message, Map<string, mixed> $data): bool;
}
