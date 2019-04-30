<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Write;

use Zynga\Framework\ShardedDatabase\V3\Config\Mysql\Base;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

class Production extends Base {

  const string DB_USERNAME = 'pokeruser';
  const string DB_PASSWORD = 'pokerpass';
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
          self::DB_USERNAME,
          self::DB_PASSWORD,
          sprintf(
            "poker-adb-fb-shard%d.cluster-cnuv0cm5n7gk.us-west-2.rds.amazonaws.com",
            $i,
          ),
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
