<?hh // strict

namespace Zynga\Framework\Database\V2\Driver\Mock;

use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;

class Transaction implements TransactionInterface {

  public function __construct(DriverInterface $driver) {}

  public function begin(): bool {
    return true;
  }

  public function commit(): bool {
    return true;
  }

  public function rollback(): bool {
    return true;
  }

}
