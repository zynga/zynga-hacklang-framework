<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces\PgModel;

use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;

interface WriterInterface {
  public function add(PgRowInterface $row): bool;
  public function save(PgRowInterface $row): bool;
  public function deleteByPk<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
    mixed $id,
  ): bool;
}
