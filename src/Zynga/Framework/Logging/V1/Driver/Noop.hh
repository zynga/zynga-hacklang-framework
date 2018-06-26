<?hh // strict

namespace Zynga\Framework\Logging\V1\Driver;

use Zynga\Framework\Logging\V1\Driver\Base as DriverBase;

class Noop extends DriverBase {

  public function writeLogEntry(int $level, string $message, Map<string, mixed> $data): bool {
    return true;
  }

}
