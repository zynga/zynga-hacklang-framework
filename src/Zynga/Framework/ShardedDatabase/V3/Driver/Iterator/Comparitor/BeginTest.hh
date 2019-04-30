<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Comparitor;

use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Comparitor\Begin as BeginComparitorDriver;
use Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Comparitor\Exception\OperationOnComparitor as OperationOnComparitorException;
use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Poker\Type\Snid\V1\BoxFactory as SnidBoxFactory;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;

class BaseTest extends TestCase {
    public function testGetConfig(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->getConfig();
    }

    public function testQuote(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->quote();
    }

    public function testTransaction(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->transaction();
    }

    public function testHasValidSocialNetworkConditions(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->hasValidSocialNetworkConditions(SnidBoxFactory::facebook(), new UidBox(1));
    }

    public function testSetSnUid(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->setSnUid(SnidBoxFactory::facebook(), new UidBox(1));
    }

    public function testGetSnUid(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->getSnUid();
    }

    public function testSetIsConnected(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->setIsConnected(true);
    }

    public function testGetIsConnected(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        //calls helper function and base function for 100% coverage
        $beginningDriver->isConnected();
    }

    public function testIsSqlDML(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->isSqlDML("");
    }

    public function testGetQuoter(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->getQuoter();
    }

    public function testGetTransaction(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->getTransaction();
    }

    public function testConnectToShard(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->connectToShard(1, "");
    }

    public function testConnect(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->connect();
    }

    public function testDisconnect(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->disconnect();
    }

    public function testOnDriverConnectionChange(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->onDriverConnectionChange(true, true);
    }

    public function testHadError(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->hadError();
    }

    public function testGetLastError(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->getLastError();
    }

    public function testQuery(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->query("");
    }

    public function testNativeQuoteString(): void {
        $driver = DatabaseFactory::getReadWithoutSnUid();
        $beginningDriver = new BeginComparitorDriver($driver->getConfig());
        $this->expectException(
            OperationOnComparitorException::class
        );
        $beginningDriver->nativeQuoteString("");
    }
}
