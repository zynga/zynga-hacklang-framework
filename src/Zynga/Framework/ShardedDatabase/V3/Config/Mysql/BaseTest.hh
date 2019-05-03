<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Mysql;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\ShardedDatabase\V3\Config\Mysql\Base;
use
  Zynga\Framework\ShardedDatabase\V3\Exceptions\ShardsInitFailureException
;
use
  Zynga\Framework\ShardedDatabase\V3\Exceptions\ShardsInitNoServersException
;
use
  Zynga\Framework\ShardedDatabase\V3\Exceptions\InvalidShardIdException
;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\UInt64Box;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

class Mock_InitFailure extends Base<UInt64Box> {
  public function shardsInit(): bool {
    return false;
  }
  public function getServerFromShardType(UInt64Box $shardType): ConnectionDetails {
    return new ConnectionDetails('', '', '', 0);
  }
  public function isDatabaseReadOnly(): bool {
    return false;
  }
  public function getShardId(UInt64Box $shardType): int {
    return 1;
  }
  public function getShardCount(): int {
    return 1;
  }
  public function getDatabaseName(): string {
    return 'Mock';
  }
}

class Mock_InitFailureNoServers extends Base<UInt64Box> {
  public function shardsInit(): bool {
    return true;
  }
  public function getServerFromShardType(UInt64Box $shardType): ConnectionDetails {
    return new ConnectionDetails('', '', '', 0);
  }
  public function isDatabaseReadOnly(): bool {
    return false;
  }
  public function getShardId(UInt64Box $shardType): int {
    return 1;
  }
  public function getShardCount(): int {
    return 1;
  }
  public function getDatabaseName(): string {
    return 'Mock';
  }
}

class Mock_SomeWhatValid extends Base<UInt64Box> {
  public function shardsInit(): bool {
    $this->addServer(
      new ConnectionDetails('someuser', 'somepass', 'a-server.com', 12367),
    );
    return true;
  }
  public function getServerFromShardType(UInt64Box $shardType): ConnectionDetails {
    return new ConnectionDetails('', '', '', 0);
  }
  public function isDatabaseReadOnly(): bool {
    return false;
  }
  public function getShardId(UInt64Box $shardType): int {
    return 1;
  }
  public function getShardCount(): int {
    return 1;
  }
  public function getDatabaseName(): string {
    return 'Mock';
  }
}

class BaseTest extends TestCase {

  public function testShardsInitFailure(): void {
    $this->expectException(ShardsInitFailureException::class);
    $obj = new Mock_InitFailure();
    // $obj->init();
  }

  public function testShardsInitFailureNoServers(): void {
    $this->expectException(ShardsInitNoServersException::class);
    $obj = new Mock_InitFailureNoServers();
    // $obj->init();
  }

  public function testShardsInvalidGetServerByOffset(): void {
    $obj = new Mock_SomeWhatValid();
    $obj->init();
    $this->expectException(InvalidShardIdException::class);
    $obj->getServerByOffset(13);
  }

  public function testClearAndCount(): void {
    $obj = new Mock_SomeWhatValid();
    $obj->init();

    $this->assertEquals(1, $obj->getServerCount());
    $this->assertTrue($obj->clearServers());
    $this->assertEquals(0, $obj->getServerCount());

  }

}
