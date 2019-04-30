<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Write;

use Zynga\Framework\ShardedDatabase\V3\Config\Mysql\Base;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

class Production extends Base {

  const string DB_PASSWORD = 'pRdLPS2h4J';
  const string DB_HOSTNAME = '127.0.0.1';
  const int PROD_SHARD_COUNT = 16;
  const int PORT = 3306;

  public function isDatabaseReadOnly(): bool {
    return false;
  }

  public function shardsInit(): bool {
    // you have to add the servers in the order of the sharding.
    for ($i = 0; $i < self::PROD_SHARD_COUNT; $i++) {
      $this->addServer(
        new ConnectionDetails(
          sprintf('sexyHack%dW', $i),
          self::DB_PASSWORD,
          self::DB_HOSTNAME,
          self::PORT,
        ),
      );
    }

    return true;
  }

  public function getShardCount(): int {
    return self::PROD_SHARD_COUNT;
  }
}
