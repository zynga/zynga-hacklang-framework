<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Read;

use Zynga\Framework\ShardedDatabase\V3\Config\Mysql\Base;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

class Dev extends Base {

  const string DB_PASSWORD = 'WPzc8H99Zx';
  const string DB_HOSTNAME = '127.0.0.1';
  const int DEV_SHARD_COUNT = 2;
  const int PORT = 3306;

  public function isDatabaseReadOnly(): bool {
    return true;
  }

  public function shardsInit(): bool {
    $this->addServer(
      new ConnectionDetails(
        'sexyHack0R',
        self::DB_PASSWORD,
        self::DB_HOSTNAME,
        self::PORT,
      ),
    );

    $this->addServer(
      new ConnectionDetails(
        'sexyHack1R',
        self::DB_PASSWORD,
        self::DB_HOSTNAME,
        self::PORT,
      ),
    );

    return true;
  }

  public function getShardCount(): int {
    return self::DEV_SHARD_COUNT;
  }
}
