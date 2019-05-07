<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces\PgModel;

use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;

interface DataInterface {

  public function createRowObjectFromClassName<TModelClass as PgRowInterface>(
    classname<TModelClass> $model,
  ): PgRowInterface;

  public function hydrateDataToRowObject<TModelClass>(
    PgRowInterface $obj,
    Map<string, mixed> $rawData,
  ): bool;

}
