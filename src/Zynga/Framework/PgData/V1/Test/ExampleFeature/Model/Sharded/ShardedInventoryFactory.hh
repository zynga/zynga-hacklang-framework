<?hh //strict

namespace Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\Sharded;

use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\PgData\V1\PgShardedModelFactory;
use
  Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\Sharded\ShardedInventoryModel
;
use Zynga\Framework\Type\V1\UInt64Box;

class ShardedInventoryFactory extends PgShardedModelFactory {

  public function __construct(private UInt64Box $_shardKey) {
    parent::__construct($_shardKey);
  }

  protected function getRealModel(): ShardedInventoryModel {
    return new ShardedInventoryModel($this->_shardKey);
  }
}
