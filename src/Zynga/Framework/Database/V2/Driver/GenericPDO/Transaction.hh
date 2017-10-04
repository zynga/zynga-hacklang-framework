<?hh // strict

namespace Zynga\Framework\Database\V2\Driver\GenericPDO;

use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;

/**
 * A GenericPDO specific implementation of ResultSet
 */
class Transaction implements TransactionInterface {
    private DriverInterface $driver;

    /**
     * Initializes this instance of Transaction
     *
     * @param $driver Driver associated with this Transaction
     */
    public function __construct(DriverInterface $driver) {
        $this->driver = $driver;
    }

    /**
     * See @TransactionInterface
     */
    public function begin(): bool {
        $this->execute('START TRANSACTION');
        return true;
    }

    /**
     * See @TransactionInterface
     */
    public function commit(): bool {
        $this->execute('COMMIT');
        return true;
    }

    /**
     * See @TransactionInterface
     */
    public function rollback(): bool {
        $this->execute('ROLLBACK');
        return true;
    }

    /**
     * Executes a sql query
     *
     * @param $query SQL query to execute
     */
    protected function execute(string $query): void {
        $this->driver->query($query);
    }
}
