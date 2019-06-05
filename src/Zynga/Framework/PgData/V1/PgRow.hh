<?hh // strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\Exceptions\InvalidPrimaryKeyException;
use Zynga\Framework\StorableObject\V1\Base as StorableObject;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

abstract class PgRow extends StorableObject implements PgRowInterface {
  private PgModelInterface $_pgModel;

  public function __construct(PgModelInterface $pgModel) {

    parent::__construct();

    $this->_pgModel = $pgModel;

  }

  public function pgModel(): PgModelInterface {
    return $this->_pgModel;
  }

  abstract public function getTableName(): string;

  abstract public function getPrimaryKey(): string;

  public function getPrimaryKeyTyped(): TypeInterface {

    $field = $this->fields()->getTypedField($this->getPrimaryKey());

    if ($field instanceof TypeInterface) {
      return $field;
    }

    throw new InvalidPrimaryKeyException(
      'Unable to find primaryKey='.
      $this->getPrimaryKey().
      ' on pgRow='.
      get_class($this),
    );

  }

  public function save(): bool {
    return $this->pgModel()->writer()->save($this);
  }
  
  public function delete(): bool {
    return $this->pgModel()->writer()->delete($this);
  }

}
