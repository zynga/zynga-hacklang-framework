<?hh // strict

namespace Zynga\Framework\PgData\V1\PgModel;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Lockable\Cache\V1\Factory as LockableCacheFactory;
use
  Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface as LockableDriverInterface
;
use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\CacheInterface;

class Cache implements CacheInterface {
  private PgModelInterface $_pgModel;

  public function __construct(PgModelInterface $pgModel) {
    $this->_pgModel = $pgModel;
  }

  private function pgModel(): PgModelInterface {
    return $this->_pgModel;
  }

  public function getDataCache(): LockableDriverInterface {
    try {
      $pgModel = $this->pgModel();
      return LockableCacheFactory::factory(
        LockableDriverInterface::class,
        $pgModel->getDataCacheName(),
      );
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function getResultSetCache(): LockableDriverInterface {
    try {
      $pgModel = $this->pgModel();
      return LockableCacheFactory::factory(
        LockableDriverInterface::class,
        $pgModel->getResultSetCacheName(),
      );
    } catch (Exception $e) {
      throw $e;
    }
  }

}
