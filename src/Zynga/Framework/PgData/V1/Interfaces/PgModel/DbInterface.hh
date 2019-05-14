<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces\PgModel;

use
  Zynga\Framework\Database\V2\Interfaces\DriverInterface as DatabaseDriverInterface
;

interface DbInterface {
  public function getReadDatabase(): DatabaseDriverInterface;
  public function getWriteDatabase(): DatabaseDriverInterface;
   public function quoteValue(
    DatabaseDriverInterface $dbh,
    mixed $value,
  ): string;
}
