<?hh // strict

namespace Zynga\Framework\Datadog\V2;

class ServiceStatus {
  const OK = 0;
  const WARNING = 1;
  const CRITICAL = 2;
  const UNKNOWN = 3;

  public static function isValidStatus(int $status): bool {
    if ($status === self::OK ||
        $status === self::WARNING ||
        $status === self::CRITICAL ||
        $status === self::UNKNOWN) {
      return true;
    }
    return false;
  }

}
