<?hh // strict 

namespace Zynga\Framework\Logging\V1\Adapter;

use Zynga\Framework\Logging\V1\Interfaces\LoggerAdapterInterface;

abstract class Base implements LoggerAdapterInterface {
  abstract public function writeLogEntry(int $level, string $message, Map<string, mixed> $data): bool;
}
