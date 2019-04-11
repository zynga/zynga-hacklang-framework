<?hh //strict

namespace Zynga\Framework\Cache\V2\Driver;

use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Cache\V2\Driver\Memcache as MemcacheDriver;

use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidStorableObject
;

class MemcacheTest extends BaseMemcacheTest {

  public function getDriverName(): string {
    return 'Mock';
  }

  public function getNonstorableDriverName(): string {
    return 'Mock_NonLockingCache_NonStorableObject';
  }

  public function getNoServersConfiguredDriverName(): string {
    return 'Mock_NonLockingCache_NoServersConfigured';
  }
}
