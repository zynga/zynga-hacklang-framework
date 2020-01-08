<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces\PgModel;

interface StatsInterface {
  public function incrementCacheHits(): bool;
  public function getCacheHits(): int;
  public function incrementCacheMisses(): bool;
  public function getCacheMisses(): int;
  public function incrementSqlSelects(): bool;
  public function getSqlSelects(): int;
}
