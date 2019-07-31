<?hh // strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgWhereClauseInterface;
use Zynga\Framework\StorableObject\Collections\Vector\V1\Base;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

/**
 * Cacheable container for just the pk values for the objects we are going to store.
 */
// TODO: Add interface
class PgCachedResultSet<Tv as TypeInterface> extends Base<Tv> {
  private PgWhereClauseInterface $_where;
  private classname<Tv> $_rawType;
  private classname<PgRowInterface> $_model;
  private string $_checksum;

  public function __construct(
    classname<Tv> $rawType,
    PgWhereClauseInterface $where,
    classname<PgRowInterface> $model
  ) {

    parent::__construct($rawType);

    $this->_where = $where;
    $this->_rawType = $rawType;
    $this->_checksum = '';
    $this->_model = $model;

  }

  public function createChecksum(): string {

    if ($this->_checksum !== '') {
      return $this->_checksum;
    }

    $whereChecksum = $this->_where->createWhereChecksum();
    $this->_checksum =
      md5($this->_rawType.'|'.$whereChecksum.'|'.$this->_model);

    return $this->_checksum;

  }

}
