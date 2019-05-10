<?hh // strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\StorableObject\Collections\Vector\V1\Base;

use Zynga\Framework\PgData\V1\Interfaces\PgResultSetInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgWhereClauseInterface;

class PgResultSet<Tv as PgRowInterface> extends Base<Tv>
  implements PgResultSetInterface<Tv> {
  private PgWhereClauseInterface $_where;
  private classname<Tv> $_rawType;
  private string $_checksum;

  public function __construct(
    private classname<Tv> $rawType,
    PgWhereClauseInterface $where,
  ) {

    // Stand up the storable vector with the default type for this item.
    parent::__construct($rawType);

    $this->_where = $where;
    $this->_rawType = $rawType;
    $this->_checksum = '';

  }

  public function createChecksum(): string {

    if ($this->_checksum !== '') {
      return $this->_checksum;
    }

    $rawType = $this->_rawType;
    $typeChecksum = md5($this->_rawType);
    $whereChecksum = $this->_where->createWhereChecksum();

    error_log(
      'queryChecksum rawType='.$rawType.' whereChecksum='.$whereChecksum,
    );

    $this->_checksum = $typeChecksum.'|'.$whereChecksum;

    return $this->_checksum;

  }

}
