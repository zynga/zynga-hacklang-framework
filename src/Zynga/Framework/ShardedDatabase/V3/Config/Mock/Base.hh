<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Mock;

use Zynga\Framework\ShardedDatabase\V3\Config\Base as ConfigBase;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;
use Zynga\Framework\Type\V1\UInt64Box;
abstract class Base extends ConfigBase<UInt64Box> {

  const string SERVER_USERNAME = 'mockuser';
  const string SERVER_PASSWORD = 'mockpass';
  const int SERVER_PORT = 1337;
  const string CURRENT_DATABASE = 'mockdb';
  const string CURRENT_SERVER = 'mockhost';

  public function shardsInit(): bool {
    $this->setCurrentDatabase(self::CURRENT_DATABASE);
    $this->setCurrentServer(self::CURRENT_SERVER);

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

  public function getDriver(): string {
    return 'Mock';
  }

  public function isDatabaseReadOnly(): bool {
    return false;
  }

  public function getConnectionStringForServer(
    UInt64Box $shardType,
    ConnectionDetails $server,
  ): string {
    $connectionString = '';
    $connectionString .= 'username='.self::SERVER_USERNAME;
    $connectionString .= 'password='.self::SERVER_PASSWORD;
    return $connectionString;
  }

  public function getServerFromShardType(UInt64Box $shardType): ConnectionDetails {
    return $this->getServerByOffset($this->getShardId($shardType));
  }

  public function getShardId(UInt64Box $shardType): int {
    return parent::getShardId($shardType);
  }

  public function getShardCount(): int {
    return 1;
  }
  
  public function getDatabaseName():string {
    return 'Mock';
  }
}
