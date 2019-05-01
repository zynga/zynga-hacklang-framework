<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

interface PgRowInterface extends StorableObjectInterface {
  public function getPrimaryKey(): string;
  public function getTableName(): string;
}
