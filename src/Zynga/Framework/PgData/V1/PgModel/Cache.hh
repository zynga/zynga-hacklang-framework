<?hh // strict

namespace Zynga\Framework\PgData\V1\PgModel;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Lockable\Cache\V1\Factory as LockableCacheFactory;
use
  Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface as LockableDriverInterface
;
use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\CacheInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgWhereClauseInterface;

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
  
  public function lockRowCache(PgRowInterface $row): bool {

    try {
      $cache = $this->getDataCache();

      return $cache->lock($row);

    } catch (Exception $e) {
      throw $e;
    }
  }
  
  public function unlockRowCache(PgRowInterface $row): bool {

    try {
      $cache = $this->getDataCache();

      return $cache->unlock($row);

    } catch (Exception $e) {
      throw $e;
    }

  }
  
  public function clearResultSetCache<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    PgWhereClauseInterface $where,
  ): bool {
    try {
      $pgModel = $this->pgModel();

      // Create a cachedRs out of the result set that was given.
      $cachedRs = $pgModel->reader()->createCachedResultSet($model, $where);

      $cache = $this->getResultSetCache();

      if($cache->lock($cachedRs)) {
        return $cache->delete($cachedRs, true);
      }

      return false;
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function lockResultSetCache<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    PgWhereClauseInterface $where,
  ): bool {
    try {
      $pgModel = $this->pgModel();

      // Create a cachedRs out of the result set that was given.
      $cachedRs = $pgModel->reader()->createCachedResultSet($model, $where);

      $cache = $this->getResultSetCache();

      return $cache->lock($cachedRs);

    } catch (Exception $e) {
      throw $e;
    }
  }

  public function unlockResultSetCache<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    PgWhereClauseInterface $where,
  ): bool {
    try {
      $pgModel = $this->pgModel();

      // Create a cachedRs out of the result set that was given.
      $cachedRs = $pgModel->reader()->createCachedResultSet($model, $where);

      $cache = $this->getResultSetCache();

      return $cache->unlock($cachedRs);

    } catch (Exception $e) {
      throw $e;
    }
  }
}
