<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces\PgModel;

use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;

interface DataInterface {

  public function createRowObjectFromClassName<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
  ): PgRowInterface;

  public function getPkFromClassName<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
  ): string;

  public function hydrateDataToRowObject<TModelClass>(
    PgRowInterface $obj,
    Map<string, mixed> $rawData,
  ): bool;

}
