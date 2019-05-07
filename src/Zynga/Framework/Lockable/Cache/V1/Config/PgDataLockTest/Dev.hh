<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1\Config\PgDataLockTest;

use
  Zynga\Framework\Cache\V2\Interfaces\DriverInterface as CacheDriverInterface
;
use Zynga\Framework\Cache\V2\Factory as CacheFactory;
use Zynga\Framework\Lockable\Cache\V1\Config\Base;
use Zynga\Framework\Lockable\Cache\V1\Driver\Caching;
use Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface;

// use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
// use
//   Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidExampleObject
// ;

use Zynga\Framework\Exception\V1\Exception;

class Dev extends Base {

  public function getCache(): CacheDriverInterface {
    return CacheFactory::factory(
      CacheDriverInterface::class,
      'LocalMemcache\PgDataLockTest',
    );
  }

  public function getDriver(): string {
    return 'Caching';
  }

  public function getLockTTL(): int {
    return 10;
  }

}
