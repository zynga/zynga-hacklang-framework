<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO;

use Zynga\Framework\Database\V2\Exceptions\InvalidShardException;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO\ConnectionContainer;
use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\Type\V1\UInt64Box;
use Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO as GenericPDODriver;
use Zynga\Framework\ShardedDatabase\V3\Driver\Mock as MockDriver;
use Zynga\Framework\Database\V2\Exceptions\QueryFailedException;
use Zynga\Framework\Database\V2\Exceptions\Mock\BadResultOffsetException;

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
    $dbh = DatabaseFactory::factory(DriverInterface::class, 'Test\Mysql');

    $testShard = new UInt64Box(1);
    $config = $dbh->getConfig();

    $dsn = $config->getConnectionString($testShard);

    $server = $config->getServerFromShardType($testShard);

    $hostname = $server->getHostname();
    $username = $server->getUsername();
    $password = $server->getPassword();

    $this->assertEquals('localhost', $hostname);
    $this->assertEquals('zframework', $username);
    $this->assertEquals('i-am-a-walrus', $password);

    $shardId = $config->getShardId($testShard);

    $con = new ConnectionContainer();

    $this->assertEquals(0, $shardId);
    $this->assertEquals(
      'mysql:host=localhost;user=zframework;password=i-am-a-walrus;port=3306;dbname=phpunit;',
      $dsn,
    );

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
    $dbh = DatabaseFactory::factory(MockDriver::class, 'Mock');
    $dbh->onDriverConnectionChange(true, true);

    $config = $dbh->getConfig();
    $testShard = new UInt64Box(1);
    $dbh->setShardType($testShard);
    $dsn = $config->getConnectionString($testShard);
    $shardId = $config->getShardId($testShard);
    $this->expectException(BadResultOffsetException::class);
    GenericPDODriver::$FORCE_COVERAGE = true;
    $dbh->query("Error");

    GenericPDODriver::$FORCE_COVERAGE = false;
  }

}
