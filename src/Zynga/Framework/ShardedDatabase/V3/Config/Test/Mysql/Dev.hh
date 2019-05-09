<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Test\Mysql;

use Zynga\Framework\ShardedDatabase\V3\Config\Mysql\Base as MysqlBase;
use Zynga\Framework\Type\V1\UInt64Box as TestId;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

class Dev extends MysqlBase<TestId> {
  const int SERVER_PORT = 3306;
  const string SERVER_USERNAME = 'zframework';
  const string SERVER_PASSWORD = 'i-am-a-walrus';
  const string SCHEMA = 'phpunit';

  public function shardsInit(): bool {
    $this->addServer(
      new ConnectionDetails(
        self::SERVER_USERNAME,
        self::SERVER_PASSWORD,
        'localhost',
        self::SERVER_PORT,
      ),
    );
    return true;
  }

  public function isDatabaseReadOnly(): bool {
    return false;
  }

  public function getDatabaseName(): string {
    return self::SCHEMA;
  }

}
