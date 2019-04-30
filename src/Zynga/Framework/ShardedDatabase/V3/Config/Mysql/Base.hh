<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Mysql;

use
  Zynga\Framework\ShardedDatabase\V3\Config\Base as ConfigShardedBase
;

use Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO\Quoter;
use
  Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO\Transaction
;
use Zynga\Framework\ShardedDatabase\V3\Info as ShardInfo;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\QuoteInterface;
use
  Zynga\Framework\ShardedDatabase\V3\Interfaces\TransactionInterface
;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;

abstract class Base extends ConfigShardedBase {

  public function getDriver(): string {
    return 'GenericPDO';
  }

  public function getConnectionStringForServer(
    SnidBox $sn,
    ConnectionDetails $server,
  ): string {
    $database = ShardInfo::getDatabaseSchemaForSocialNetworkId($sn);
    $hostname = $server->getHostname();
    $port = $server->getPort();
    $this->setCurrentDatabase($database);
    $this->setCurrentServer($hostname);
    $dsn = '';
    $dsn = 'mysql:host='.$hostname.';dbname='.$database.';'.$port;
    return $dsn;
  }

  public function getServerFromUserId(SnidBox $sn, UidBox $uid): ConnectionDetails {
    $shardIndex = $this->getShardId($sn, $uid);
    return $this->getServerByOffset($shardIndex);
  }

  public function getShardId(SnidBox $sn, UidBox $uid): int {
    $shardCount = $this->getServerCount();
    $shardOffset = $uid->get() % $shardCount;
    return $shardOffset;
  }
}
