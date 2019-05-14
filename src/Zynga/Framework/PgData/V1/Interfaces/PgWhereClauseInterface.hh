<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces;

use Zynga\Framework\Database\V2\Interfaces\QueryableInterface;
use Zynga\Framework\PgData\V1\PgWhereOperand;

interface PgWhereClauseInterface {

  public function and(
    string $field,
    PgWhereOperand $operand,
    mixed $value,
  ): bool;

  public function or(
    string $field,
    PgWhereOperand $operand,
    mixed $value,
  ): bool;

  public function count(): int;

  public function buildSql(
    QueryableInterface $dbh,
    PgRowInterface $row,
  ): string;

  public function createWhereChecksum(): string;

}
