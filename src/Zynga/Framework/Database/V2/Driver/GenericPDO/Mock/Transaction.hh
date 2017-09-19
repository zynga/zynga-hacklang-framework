<?hh // strict

namespace Zynga\Framework\Database\V2\Driver\GenericPDO\Mock;

use Zynga\Framework\Database\V2\Driver\GenericPDO\Transaction as BaseTransaction;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;

/**
 * A GenericPDO specific implementation of Transaction
 */
class Transaction extends BaseTransaction {
    protected function execute(string $query): void {
    }
}
