<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Mock;

use Zynga\Framework\ShardedDatabase\V3\Interfaces\TransactionInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

class Transaction<TType as TypeInterface>  implements TransactionInterface<TType> {

  public function __construct(DriverInterface<TType> $driver) {}

  public function begin(int $sn = 0, int $uid = 0): bool {
    return true;
  }

  public function commit(int $sn = 0, int $uid = 0): bool {
    return true;
  }

  public function rollback(int $sn = 0, int $uid = 0): bool {
    return true;
  }

}
