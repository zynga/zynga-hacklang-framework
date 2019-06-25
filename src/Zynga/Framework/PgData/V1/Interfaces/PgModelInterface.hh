<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces;

use Zynga\Framework\PgData\V1\Interfaces\PgModel\CacheInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\DataInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\DbInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\ReaderInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\StatsInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\WriterInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;

interface PgModelInterface {

  public function add(PgRowInterface $row, bool $shouldUnlock = true): bool;
  public function cache(): CacheInterface;
  public function data(): DataInterface;
  public function db(): DbInterface;
  public function getDataCacheName(): string;
  public function getResultSetCacheName(): string;
  public function getReadDatabaseName(): string;
  public function getWriteDatabaseName(): string;
  public function reader(): ReaderInterface;
  public function stats(): StatsInterface;
  public function writer(): WriterInterface;
  public function getByPk<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    mixed $id,
    bool $shouldLock
  ): ?PgRowInterface;
  public function get<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    ?PgWhereClauseInterface $where = null,
  ): PgResultSetInterface<PgRowInterface>;
}
