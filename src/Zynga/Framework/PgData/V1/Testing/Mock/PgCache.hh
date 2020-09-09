<?hh //strict

namespace Zynga\Framework\PgData\V1\Testing\Mock;

use Zynga\Framework\PgData\V1\Interfaces\PgModel\ReaderInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\StatsInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\WriterInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\Testing\Mock\PgReaderWriter;
use Zynga\Framework\PgData\V1\PgModel\Cache as Base;
use Zynga\Framework\StorableObject\Collections\Map\V2\Base as StorableMap;
use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;

/**
 * A Mock PGModel which uses the PGReaderWriter instead of direct interactions with
 *  the database.
 */
class PgCache extends Base {

  public static Map<string, bool> $lockvalues = Map {};

  public static function lockSetValue(PgRowInterface $row, bool $value): void {
    if(self::$lockvalues === null) {
      self::$lockvalues = Map {};
    }

    self::$lockvalues->set(self::getKey($row), $value);
  }

  public function lockRowCache(PgRowInterface $row): bool {
    if(self::$lockvalues->containsKey(self::getKey($row))) {
      return self::$lockvalues->at(self::getKey($row));
    }

    return parent::lockRowCache($row);
  }

  public static function getKey(PgRowInterface $row): string {
    return md5(get_class($row) . (string)$row->getPrimaryKeyTyped()->get());
  }
}
