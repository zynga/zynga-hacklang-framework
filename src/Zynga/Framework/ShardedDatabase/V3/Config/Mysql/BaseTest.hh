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

use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

class Mock_InitFailure extends Base {
  public function shardsInit(): bool {
    return false;
  }
  public function getServerFromUserId(SnidBox $sn, UidBox $uid): ConnectionDetails {
    return new ConnectionDetails('', '', '', 0);
  }
  public function isDatabaseReadOnly(): bool {
    return false;
  }
  public function getShardId(SnidBox $sn, UidBox $uid): int {
    return 1;
  }
  public function getShardCount(): int {
    return 1;
  }
}

class Mock_InitFailureNoServers extends Base {
  public function shardsInit(): bool {
    return true;
  }
  public function getServerFromUserId(SnidBox $sn, UidBox $uid): ConnectionDetails {
    return new ConnectionDetails('', '', '', 0);
  }
  public function isDatabaseReadOnly(): bool {
    return false;
  }
  public function getShardId(SnidBox $sn, UidBox $uid): int {
    return 1;
  }
  public function getShardCount(): int {
    return 1;
  }
}

class Mock_SomeWhatValid extends Base {
  public function shardsInit(): bool {
    $this->addServer(
      new ConnectionDetails('someuser', 'somepass', 'a-server.com', 12367),
    );
    return true;
  }
  public function getServerFromUserId(SnidBox $sn, UidBox $uid): ConnectionDetails {
    return new ConnectionDetails('', '', '', 0);
  }
  public function isDatabaseReadOnly(): bool {
    return false;
  }
  public function getShardId(SnidBox $sn, UidBox $uid): int {
    return 1;
  }
  public function getShardCount(): int {
    return 1;
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
