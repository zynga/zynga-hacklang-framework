<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Mock;

use Zynga\Framework\ShardedDatabase\V3\Interfaces\TransactionInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;

class Transaction implements TransactionInterface {

  public function __construct(DriverInterface $driver) {}

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
