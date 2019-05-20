<?hh // strict

namespace Zynga\Framework\PgData\V1\Sharded;

use Zynga\Framework\PgData\V1\PgModel as PgModelBase;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\DbInterface;
use Zynga\Framework\PgData\V1\Interfaces\Sharded\PgModelInterface;
use Zynga\Framework\PgData\V1\Sharded\PgModel\Db;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

abstract class PgModel extends PgModelBase implements PgModelInterface {
  private TypeInterface $_shardId;

  public function __construct(TypeInterface $shardId) {
    $this->_shardId = $shardId;

  }

  public function getShardId(): TypeInterface {

    return $this->_shardId;
  }

  public function createDbObject(): DbInterface {
    return new Db($this, $this->getShardId());
  }

}
