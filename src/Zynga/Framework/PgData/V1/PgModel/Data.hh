<?hh // strict

namespace Zynga\Framework\PgData\V1\PgModel;

use Zynga\Framework\Dynamic\V1\DynamicClassCreation;
use Zynga\Framework\PgData\V1\Exceptions\PgRowInterfaceRequiredException;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\DataInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\Exception\V1\Exception;

class Data implements DataInterface {
  private PgModelInterface $_pgModel;

  public function __construct(PgModelInterface $pgModel) {
    $this->_pgModel = $pgModel;
  }

  private function pgModel(): PgModelInterface {
    return $this->_pgModel;
  }

  public function createRowObjectFromClassName<TModelClass as PgRowInterface>(
    classname<TModelClass> $row,
  ): PgRowInterface {

    try {
      $obj = DynamicClassCreation::createClassByNameGeneric(
        $row,
        Vector {$this->pgModel()},
      );

      if ($obj instanceof PgRowInterface) {
        return $obj;
      }

      throw new PgRowInterfaceRequiredException('rowProvided='.$row);

    } catch (Exception $e) {
      throw $e;
    }

  }

  public function hydrateDataToRowObject<TModelClass>(
    PgRowInterface $obj,
    Map<string, mixed> $rawData,
  ): bool {
    try {
      return $obj->import()->fromMap($rawData);
    } catch (Exception $e) {
      throw $e;
    }
  }

}
