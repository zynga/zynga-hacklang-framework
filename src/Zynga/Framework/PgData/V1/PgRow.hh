<?hh // strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\Exceptions\InvalidPrimaryKeyException;
use Zynga\Framework\StorableObject\V1\Base as StorableObject;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\BoolBox;

abstract class PgRow extends StorableObject implements PgRowInterface {
  private PgModelInterface $_pgModel;
  public BoolBox $_ignore_tombstoned;

  public function __construct(PgModelInterface $pgModel) {

    parent::__construct();

    $this->_ignore_tombstoned = new BoolBox();
    $this->_ignore_tombstoned->set(false);
    $this->_ignore_tombstoned->setIsRequired(false);

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

  public function save(bool $shouldUnlock = true): bool {
    return $this->pgModel()->writer()->save($this, $shouldUnlock);
  }

  public function delete(bool $shouldUnlock = true): bool {
    return $this->pgModel()->writer()->delete($this, $shouldUnlock);
  }

  public function isTombstoned(): bool {
    return $this->_ignore_tombstoned->get();
  }

  public function tombstoneRow(): bool {
    $this->_ignore_tombstoned->set(true);
    return $this->_ignore_tombstoned->get();
  }

  public function getCacheKeyPrefix(): string {
    return "";
  }

}
