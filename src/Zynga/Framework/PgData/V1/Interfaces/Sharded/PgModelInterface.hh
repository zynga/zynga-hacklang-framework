<?hh // strict

namespace Zynga\Framework\PgData\V1\Interfaces\Sharded;

use
  Zynga\Framework\PgData\V1\Interfaces\PgModelInterface as PgModelInterfaceBase
;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

interface PgModelInterface extends PgModelInterfaceBase {
  public function getShardId(): TypeInterface;

}
