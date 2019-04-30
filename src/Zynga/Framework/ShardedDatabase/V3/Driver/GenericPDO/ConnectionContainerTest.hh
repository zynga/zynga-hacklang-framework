<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO;

use Zynga\Framework\Database\V2\Exceptions\InvalidShardException;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use
  Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO\ConnectionContainer
;
use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Poker\Type\Snid\V1\BoxFactory as SnidBoxFactory;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use
  Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO as GenericPDODriver
;
use Zynga\Framework\Database\V2\Exceptions\QueryFailedException;

class ConnectionContainerTest extends TestCase {

  public function doSetUpBeforeClass(): bool {

    parent::doSetUpBeforeClass();

    DatabaseFactory::disableMockDrivers();

    return true;

  }
  <<__Override>>
  public function tearDown(): void {
    parent::tearDown();
    GenericPDODriver::$FORCE_COVERAGE = false;
  }

  public function testCachingCreate(): void {
    $dbh = DatabaseFactory::factory(DriverInterface::class, 'Read');

    $testSn = SnidBoxFactory::facebook();
    $testUid = new UidBox(1);

    $config = $dbh->getConfig();

    $dsn = $config->getConnectionString($testSn, $testUid);
    $server = $config->getServerFromUserId($testSn, $testUid);
    $shardId = $config->getShardId($testSn, $testUid);

    $username = $server->getUsername();
    $password = $server->getPassword();

    $con = new ConnectionContainer();

    // should be new
    $this->assertTrue($con->create($shardId, $dsn, $username, $password));

    // should be cached at this point.
    $this->assertTrue($con->create($shardId, $dsn, $username, $password));
  }

  public function testInvalidShardId(): void {
    $con = new ConnectionContainer();
    $this->expectException(InvalidShardException::class);
    $con->get(1);
  }

  public function testOnDriverConnectionAndBadQuery(): void {
    $dbh = DatabaseFactory::factory(GenericPDODriver::class, 'Read');
    $dbh->onDriverConnectionChange(true, true);

    $config = $dbh->getConfig();
    $this->expectException(QueryFailedException::class);

    $testSn = SnidBoxFactory::facebook();
    $testUid = new UidBox(1);
    $dsn = $config->getConnectionString($testSn, $testUid);
    $shardId = $config->getShardId($testSn, $testUid);
    $dbh->setSnUid($testSn, $testUid);

    GenericPDODriver::$FORCE_COVERAGE = true;
    $dbh->query("Error");

    GenericPDODriver::$FORCE_COVERAGE = false;
  }

}
