<?hh // strict

namespace Zynga\Framework\PgData\V1\PgModel;

class Stats {
  private int $_cacheHits;
  private int $_cacheMisses;
  private int $_sqlSelects;

  public function __construct() {
    $this->_cacheHits = 0;
    $this->_cacheMisses = 0;
    $this->_sqlSelects = 0;
  }

  public function incrementCacheHits(): bool {
    $this->_cacheHits++;
    return true;
  }

  public function getCacheHits(): int {
    return $this->_cacheHits;
  }

  public function incrementCacheMisses(): bool {
    $this->_cacheMisses++;
    return true;
  }

  public function getCacheMisses(): int {
    return $this->_cacheMisses;
  }

  public function incrementSqlSelects(): bool {
    $this->_sqlSelects++;
    return true;
  }

  public function getSqlSelects(): int {
    return $this->_sqlSelects;
  }

}
