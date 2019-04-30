<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Comparitor;

use Zynga\Framework\ShardedDatabase\V3\Driver\Base as DriverBase;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Comparitor\Exception\OperationOnComparitor as OperationOnComparitorException;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverConfigInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\QuoteInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\ResultSetInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\TransactionInterface;
use Zynga\Environment\V1\ErrorCapture\Handler\Noop as ErrorCaptureNoop;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;

/**
 * See @Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Iterable
 */
abstract class Base extends DriverBase {
    public final function getConfig(): DriverConfigInterface {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public final function quote(): QuoteInterface {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public final function transaction(): TransactionInterface {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public final function hasValidSocialNetworkConditions(SnidBox $sn, UidBox $uid): bool {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public final function setSnUid(SnidBox $sn, UidBox $uid): bool {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public final function getSnUid():(SnidBox, UidBox) {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public final function setIsConnected(bool $state): bool {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public final function getIsConnected(): bool {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public final function isSqlDML(string $sql): bool {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public function getQuoter(): QuoteInterface {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public function getTransaction(): TransactionInterface {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public function connectToShard(int $shardIndex, string $connectionString): bool {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public function connect(): bool {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public function disconnect(): bool {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public function onDriverConnectionChange(bool $from, bool $to): void {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public function hadError(): bool {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public function getLastError(): string {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public function query(string $sql): ResultSetInterface {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }

    public function nativeQuoteString(string $value): string {
        throw new OperationOnComparitorException("Attempting to call " . __FUNCTION__ . " on a comparitor class");
    }
}
