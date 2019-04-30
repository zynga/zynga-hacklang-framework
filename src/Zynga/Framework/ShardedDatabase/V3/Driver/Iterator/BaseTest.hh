<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Iterator;

use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Base as DriverIterator;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception\CalledBeginMultipleTimes as CalledBeginMultipleTimesException;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception\DriverConnect as IteratorDriverConnectException;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception\DriverDisconnect as IteratorDriverDisconnectException;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception\UseBeforeCallToBegin as UseBeforeCallToBeginException;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Exception\OutOfBounds as IteratorOutOfBoundsException;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Mock\FailedConnect as FailedConnectMockDriverIterator;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Mock\FailedDisconnect as FailedDisconnectMockDriverIterator;
use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Poker\Type\Snid\V1\BoxFactory as SnidBoxFactory;

class BaseTest extends TestCase {
    public function testBeginReturnsValidDriver(): void {
        $iterator = DatabaseFactory::getReadIterator(SnidBoxFactory::facebook());
        $driver = $iterator->begin();
        $iterator->finish();
        $this->assertTrue($driver instanceof DriverInterface);
    }

    public function testIteratingThroughLoopReturnsValidDrivers(): void {
        for ($iterator = DatabaseFactory::getReadIterator(SnidBoxFactory::facebook()), $driver = $iterator->begin();
            $driver !== $iterator->end();
            $driver = $iterator->next()) {
            $this->assertTrue($driver instanceof DriverInterface);
        }
    }

    public function testCallingNextWithoutBeginThrowsException(): void {
        $iterator = DatabaseFactory::getReadIterator(SnidBoxFactory::facebook());
        $this->expectException(
            UseBeforeCallToBeginException::class
        );
        $iterator->next();
    }

    public function testIteratorOutOfBoundsException(): void {
        $iterator = DatabaseFactory::getReadIterator(SnidBoxFactory::facebook());
        for ($driver = $iterator->begin();
            $driver !== $iterator->end();
            $driver = $iterator->next()) {
        }

        $this->expectException(
            IteratorOutOfBoundsException::class
        );
        $iterator->next();
    }

    public function testIteratorDriverConnectException(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $iterator = new FailedConnectMockDriverIterator($driver->getConfig(), SnidBoxFactory::facebook());
        $this->expectException(
            IteratorDriverConnectException::class
        );
        $iterator->begin();
    }

    public function testIteratorDriverDisconnectException(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $iterator = new FailedDisconnectMockDriverIterator($driver->getConfig(), SnidBoxFactory::facebook());
        $iterator->begin();
        $this->expectException(
            IteratorDriverDisconnectException::class
        );
        $iterator->next();
    }

    public function testCallingBeginTwiceThrowsException(): void {
        $iterator = DatabaseFactory::getReadIterator(SnidBoxFactory::facebook());
        $iterator->begin();
        $this->expectException(
            CalledBeginMultipleTimesException::class
        );
        $iterator->begin();
    }
}
