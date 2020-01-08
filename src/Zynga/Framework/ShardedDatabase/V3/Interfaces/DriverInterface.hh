<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Interfaces;

use Zynga\Framework\Database\V2\Interfaces\QueryableInterface;
use
  Zynga\Framework\Factory\V2\Interfaces\DriverInterface as FactoryDriverInterface
;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverConfigInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\ResultSetInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

interface DriverInterface<TType as TypeInterface>
  extends FactoryDriverInterface, QueryableInterface {

  /**
   * Developer productivity optimization, we allow you to set a default uid and
   * snid to be passed to queries, and the quoting interfaces.
   * [Only used on drivers that are sharded by sn]
   *
   * @param TType $shardType
   * @return bool
   */
  public function setShardType(TType $shardType): bool;

  /**
   * Return the current default sn/uid for this driver.
   * [Only used on drivers that are sharded by sn]
   *
   * @return TType $shardType
   */
  public function getShardType(): TType;

  public function getConfig(): DriverConfigInterface<TType>;

  public function connectToShard(
    int $shardIndex,
    string $connectionString,
  ): bool;

}
