<?hh //strict

namespace Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\Sharded;

use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\PgShardedModelFactory;
use
  Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\Sharded\ShardedInventoryModel
;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

class ShardedInventoryFactory extends PgShardedModelFactory {
  protected static function getRealModel(
    TypeInterface $shardKey,
  ): PgModelInterface {
    return new ShardedInventoryModel($shardKey);
  }
}
