<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Read;

use Zynga\Framework\ShardedDatabase\V3\Config\Mysql\Base;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

class Dev extends Base {

  const string DB_USERNAME = 'pkrShdR';
  const string DB_PASSWORD = '7nzS4dZWr0';
  const int DEV_SHARD_COUNT = 1;
  const int PORT = 3306;

  public function isDatabaseReadOnly(): bool {
    return true;
  }

  public function shardsInit(): bool {
    $this->addServer(
      new ConnectionDetails(
        self::DB_USERNAME,
        self::DB_PASSWORD,
        'poker-dev-adb-fb-shard0.cluster-cnuv0cm5n7gk.us-west-2.rds.amazonaws.com',
        self::PORT,
      ),
    );

    return true;
  }

  public function getShardCount(): int {
    return self::DEV_SHARD_COUNT;
  }
}
