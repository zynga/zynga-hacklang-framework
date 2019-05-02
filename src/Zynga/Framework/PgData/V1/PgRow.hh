<?hh // strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\StorableObject\V1\Base as StorableObject;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use \Exception;

abstract class PgRow extends StorableObject implements PgRowInterface {

  abstract public function getTableName(): string;

  abstract public function getPrimaryKey(): string;

  public function getPrimaryKeyTyped(): TypeInterface {

    $field = $this->fields()->getTypedField($this->getPrimaryKey());

    if ($field instanceof TypeInterface) {
      return $field;
    }

    throw new Exception(
      'Unable to find primaryKey='.
      $this->getPrimaryKey().
      ' on pgRow='.
      get_class($this),
    );

  }

}
