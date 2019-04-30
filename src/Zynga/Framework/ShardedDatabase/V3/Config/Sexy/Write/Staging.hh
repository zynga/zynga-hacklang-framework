<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Sexy\Write;

use Zynga\Framework\ShardedDatabase\V3\Config\Mysql\Base;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

class Staging extends Base {

  const string DB_PASSWORD = 'pRdLPS2h4J';
  const string DB_HOSTNAME = '127.0.0.1';
  const int STAGING_SHARD_COUNT = 2;
  const int PORT = 3306;

  public function isDatabaseReadOnly(): bool {
    return false;
  }

  public function shardsInit(): bool {
    $this->addServer(
      new ConnectionDetails(
        'sexyHack0W',
        self::DB_PASSWORD,
        self::DB_HOSTNAME,
        self::PORT,
      ),
    );

    $this->addServer(
      new ConnectionDetails(
        'sexyHack1W',
        self::DB_PASSWORD,
        self::DB_HOSTNAME,
        self::PORT,
      ),
    );

    return true;
  }

  public function getShardCount(): int {
    return self::STAGING_SHARD_COUNT;
  }
}
