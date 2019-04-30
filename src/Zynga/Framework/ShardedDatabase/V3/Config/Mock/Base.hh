<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Mock;

use Zynga\Framework\ShardedDatabase\V3\Config\Base as ConfigBase;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

abstract class Base extends ConfigBase {

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
    SnidBox $sn,
    ConnectionDetails $server,
  ): string {
    $connectionString = '';
    $connectionString .= 'username='.self::SERVER_USERNAME;
    $connectionString .= 'password='.self::SERVER_PASSWORD;
    return $connectionString;
  }

  public function getServerFromUserId(SnidBox $sn, UidBox $uid): ConnectionDetails {
    return $this->getServerByOffset(0);
  }

  public function getShardId(SnidBox $sn, UidBox $uid): int {
    return 0;
  }

  public function getShardCount(): int {
    return 1;
  }

}
