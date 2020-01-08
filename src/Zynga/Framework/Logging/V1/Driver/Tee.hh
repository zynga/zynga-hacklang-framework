<?hh // strict

namespace Zynga\Framework\Logging\V1\Driver;

use Zynga\Framework\Logging\V1\Driver\Base as DriverBase;
use Zynga\Framework\Logging\V1\Interfaces\LoggerAdapterInterface;
use Zynga\Framework\Logging\V1\Interfaces\LoggerConfigInterface;

class Tee extends DriverBase {

  public function writeLogEntry(int $level, string $message, Map<string, mixed> $data): bool {

    $adapters = $this->getConfig()->getAdapters();

    foreach ( $adapters as $adapter ) {
      $adapter->writeLogEntry($level, $message, $data);
    }

    return true;
  }

}
