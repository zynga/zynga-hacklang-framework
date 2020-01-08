<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces\PgModel;

use Zynga\Framework\PgData\V1\Interfaces\PgResultSetInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgWhereClauseInterface;
use Zynga\Framework\PgData\V1\PgCachedResultSet;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

interface ReaderInterface {
  public function getByPk<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    mixed $id,
    bool $getLocked
  ): ?PgRowInterface;
  
  public function get<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    ?PgWhereClauseInterface $where = null,
  ): PgResultSetInterface<PgRowInterface>;
  
  public function createCachedResultSet<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    PgWhereClauseInterface $pgWhere,
  ): PgCachedResultSet<TypeInterface>;
}
