<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces;

use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use
  Zynga\Framework\StorableObject\Collections\V1\Interfaces\StorableCollection
;

interface PgResultSetInterface<Tv as PgRowInterface>
  extends StorableCollection<Tv> {
  public function createChecksum(): string;
}
