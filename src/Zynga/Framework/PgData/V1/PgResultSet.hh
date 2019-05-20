<?hh // strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\StorableObject\Collections\Vector\V1\Base;

use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgResultSetInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgWhereClauseInterface;

class PgResultSet<Tv as PgRowInterface> extends Base<Tv>
  implements PgResultSetInterface<Tv> {}
