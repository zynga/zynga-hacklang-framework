<?hh // strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\StorableObject\V1\Base as StorableObject;

abstract class PgRow extends StorableObject implements PgRowInterface {

  abstract public function getTableName(): string;

}
