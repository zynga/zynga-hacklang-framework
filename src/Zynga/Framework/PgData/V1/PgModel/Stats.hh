<?hh // strict

namespace Zynga\Framework\PgData\V1\PgModel;

use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\StatsInterface;

class Stats implements StatsInterface {
  private int $_cacheHits;
  private int $_cacheMisses;
  private int $_sqlSelects;
  private PgModelInterface $_pgModel;

  public function __construct(PgModelInterface $pgModel) {
    $this->_cacheHits = 0;
    $this->_cacheMisses = 0;
    $this->_sqlSelects = 0;
    $this->_pgModel = $pgModel;

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
