<?hh //strict

namespace Zynga\Framework\PgData\V1\Testing\Mock;

use Zynga\Framework\PgData\V1\Testing\Mock\PgModel;
use Zynga\Framework\PgData\V1\Interfaces\Sharded\PgModelInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

class PgShardedModel extends PgModel implements PgModelInterface {
  public function __construct(private TypeInterface $_shardId) {
    parent::__construct();
  }

  public function getShardId(): TypeInterface {
    return $this->_shardId;
  }

  public function setShardId(TypeInterface $_shardId): void {
    $this->_shardId = $_shardId;
  }

}
