<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Iterator;

use Zynga\Framework\ShardedDatabase\V3\Driver\Base as DriverBase;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Comparitor\Begin as BeginComparitorDriver;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Comparitor\End as EndComparitorDriver;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Iterable as DriverIterable;
use Zynga\Framework\ShardedDatabase\V3\Factory as UserShardedFactory;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverConfigInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception\CalledBeginMultipleTimes as CalledBeginMultipleTimesException;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception\DriverConnect as IteratorDriverConnectException;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception\DriverDisconnect as IteratorDriverDisconnectException;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception\OutOfBounds as IteratorOutOfBoundsException;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception\UseBeforeCallToBegin as UseBeforeCallToBeginException;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;

/**
 * Iterator that iterates over all user shards, handling connection and disconnection for the user.
 *
 * The common pattern of use is:
 * for ($iterator = DatabaseFactory::getReadIterator($sn), $driver = $iterator->begin();
 *     $driver !== $iterator->end();
 *     $driver = $iterator->next()) {
 *     $driver->foo();
 * }
 */
class Base implements DriverIterable {

    private int $currentShardIndex;
    private SnidBox $sn;
    private DriverConfigInterface $driverConfig;
    private DriverInterface $currentDriver;
    private DriverInterface $beginningDriver;
    private DriverInterface $endingDriver;
    private bool $calledBegin;

    /**
     * @param $driverConfig Configuration to use for the drivers returned from this iterator.
     * @param $sn Social network to be used for database iteration
     */
    public function __construct(DriverConfigInterface $driverConfig, SnidBox $sn) {
        $this->driverConfig = $driverConfig;
        $this->currentShardIndex = -1;
        $this->sn = $sn;
        $this->beginningDriver = new BeginComparitorDriver($driverConfig);
        $this->endingDriver = new EndComparitorDriver($driverConfig);
        $this->currentDriver = $this->beginningDriver;
        $this->calledBegin = false;
    }

    /**
     * Returns whether or not another driver is available through another call to next().
     *
     * @return True when another driver is available through another call to next(), otherwise false.
     */
    public function hasNext(): bool {
        return $this->currentShardIndex + 1 < $this->driverConfig->getShardCount();
    }

    /**
     * See @DriverIterable
     *
     * Note that there is no guaranteed ordering of the shards returned by this call. This method
     * should not be called unless begin() has been called first. Additionally, this method should
     * only be called iff hasNext() returned true immediately prior. If an exception is thrown from
     * this method, any database connection opened as a result of this class will be gracefully closed.
     *
     * @throws UseBeforeCallToBeginException if this method is called before having called begin()
     * @throws IteratorOutOfBoundsException if this method is called when there are no more drivers
     *         to be iterated upon
     * @throws IteratorDriverDisconnectException if a connected opened from a previous call to
     *         begin() or next() fails to be closed from the current call to this method.
     * @throws IteratorDriverConnectException if there is a failure connecting to the current shard
     */
    public function next(): DriverInterface {
        if ($this->calledBegin === false) {
            $this->finish();
            throw new UseBeforeCallToBeginException();
        }

        if ($this->currentDriver === $this->end()) {
            $this->finish();
            throw new IteratorOutOfBoundsException();
        }

        if ($this->currentDriver !== $this->beginningDriver) {
            if ($this->disconnectFromCurrentDriver() !== true) {
                $this->finish();
                throw new IteratorDriverDisconnectException();
            }
        }

        $this->currentDriver = $this->getNewDriver();
        if ($this->currentDriver !== $this->end()) {
            if ($this->connectToShard() !== true) {
                $this->finish();
                throw new IteratorDriverConnectException();
            }
        }

        ++$this->currentShardIndex;
        return $this->currentDriver;
    }

    /**
     * See @DriverIterable
     *
     * @throws CalledBeginMultipleTimesException
     */
    public function begin(): DriverInterface {
        if ($this->calledBegin === true) {
            throw new CalledBeginMultipleTimesException();
        }

        $this->calledBegin = true;
        return $this->next();
    }

    /**
     * See @DriverIterable
     */
    public function end(): DriverInterface {
        return $this->endingDriver;
    }

    /**
     * See @DriverIterable
     */
    public function finish(): bool {
        $this->disconnectFromCurrentDriver();
        $this->currentDriver = $this->end();
        $this->currentShardIndex = $this->driverConfig->getShardCount();
        return true;
    }

// #region Private
// These methods are intended for private use, never public. They are only exposed as public to allow for unit tests.

    public function connectToShard(): bool {
        $serverName = $this->driverConfig->getServerByOffset($this->currentShardIndex + 1);
        $connectionString = $this->driverConfig->getConnectionStringForServer($this->sn, $serverName);

        return $this->currentDriver->connectToShard($this->currentShardIndex + 1, $connectionString);
    }

    public function disconnectFromCurrentDriver(): bool {
        if ($this->currentDriver === $this->beginningDriver ||
            $this->currentDriver === $this->endingDriver) {
            return true;
        }

        return $this->currentDriver->disconnect();
    }

    public function getNewDriver(): DriverInterface {
        if ($this->hasNext()) {
           return UserShardedFactory::getReadWithoutUid($this->sn);
        }

        return $this->end();
    }

// #endregion Private
}
