<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces;

use Zynga\Framework\PgData\V1\Interfaces\PgModel\CacheInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\DataInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\DbInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\StatsInterface;

interface PgModelInterface {

  public function getDataCacheName(): string;
  public function getReadDatabaseName(): string;
  public function getWriteDatabaseName(): string;
  public function cache(): CacheInterface;
  public function data(): DataInterface;
  public function db(): DbInterface;
  public function stats(): StatsInterface;

}
