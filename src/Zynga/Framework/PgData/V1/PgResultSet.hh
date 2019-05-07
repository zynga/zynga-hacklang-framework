<?hh // strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\StorableObject\Collections\Vector\V1\Base;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;

class PgResultSet<Tv as PgRowInterface> extends Base<Tv> {}
