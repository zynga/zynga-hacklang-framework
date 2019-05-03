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
use Zynga\Framework\Type\V1\UInt64Box;

class BaseTest extends TestCase {
    public function testBeginReturnsValidDriver(): void {
        $iterator = DatabaseFactory::getMockIterator();
        $driver = $iterator->begin();
        $iterator->finish();
        $this->assertTrue($driver instanceof DriverInterface);
    }

    public function testIteratingThroughLoopReturnsValidDrivers(): void {
        for ($iterator = DatabaseFactory::getMockIterator(), $driver = $iterator->begin();
            $driver !== $iterator->end();
            $driver = $iterator->next()) {
            $this->assertTrue($driver instanceof DriverInterface);
        }
    }

    public function testCallingNextWithoutBeginThrowsException(): void {
        $iterator = DatabaseFactory::getMockIterator();
        $this->expectException(
            UseBeforeCallToBeginException::class
        );
        $iterator->next();
    }

    public function testIteratorOutOfBoundsException(): void {
        $iterator = DatabaseFactory::getMockIterator();
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
        $driver = DatabaseFactory::getMockDriver();
        $iterator = new FailedConnectMockDriverIterator($driver->getConfig(), new UInt64Box(1));
        $this->expectException(
            IteratorDriverConnectException::class
        );
        $iterator->begin();
    }

    public function testIteratorDriverDisconnectException(): void {
        $driver = DatabaseFactory::getMockDriver();
        $iterator = new FailedDisconnectMockDriverIterator($driver->getConfig(), new UInt64Box(1));
        $iterator->begin();
        $this->expectException(
            IteratorDriverDisconnectException::class
        );
        $iterator->next();
    }

    public function testCallingBeginTwiceThrowsException(): void {
        $iterator = DatabaseFactory::getMockIterator();
        $iterator->begin();
        $this->expectException(
            CalledBeginMultipleTimesException::class
        );
        $iterator->begin();
    }
}
