<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces\PgModel;

use Zynga\Framework\Database\V2\Interfaces\QueryableInterface;

interface DbInterface {
  public function getReadDatabase(): QueryableInterface;
  public function getWriteDatabase(): QueryableInterface;
  public function quoteValue(QueryableInterface $dbh, mixed $value): string;
}
