<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces\PgModel;

use
  Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface as LockableDriverInterface
;

interface CacheInterface {
  public function getDataCache(): LockableDriverInterface;
}
