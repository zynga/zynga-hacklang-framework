<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces\PgModel;

use
  Zynga\Framework\Lockable\Cache\V1\Interfaces\DriverInterface as LockableDriverInterface
;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgWhereClauseInterface;

interface CacheInterface {
  public function getDataCache(): LockableDriverInterface;
  public function getResultSetCache(): LockableDriverInterface;
  public function lockRowCache(PgRowInterface $row): bool;
  public function unlockRowCache(PgRowInterface $row): bool;
  
  public function clearResultSetCache<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    PgWhereClauseInterface $where,
  ): bool;
  
  public function lockResultSetCache<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    PgWhereClauseInterface $where,
  ): bool;
  
  public function unlockResultSetCache<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    PgWhereClauseInterface $where,
  ): bool;
}
