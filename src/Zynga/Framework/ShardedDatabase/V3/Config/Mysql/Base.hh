<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Mysql;

use Zynga\Framework\ShardedDatabase\V3\Config\Base as ConfigShardedBase;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;
use Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO\Quoter;
use Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO\Transaction;
use Zynga\Framework\ShardedDatabase\V3\Info as ShardInfo;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

abstract class Base<TType as TypeInterface> extends ConfigShardedBase<TType> {

  public function getDriver(): string {
    return 'GenericPDO';
  }

  public function getConnectionStringForServer(
    TType $shardType,
    ConnectionDetails $server,
  ): string {

    $database = $this->getDatabaseName();
    $hostname = $server->getHostname();
    $username = $server->getUsername();
    $password = $server->getPassword();
    $port = $server->getPort();

    $this->setCurrentDatabase($database);
    $this->setCurrentServer($hostname);

    $connectionString = 'mysql:';
    $connectionString .= 'host='.$hostname.';';
    $connectionString .= 'user='.$username.';';
    $connectionString .= 'password='.$password.';';
    $connectionString .= 'port='.$port.';';
    $connectionString .= 'dbname='.$database.';';

    return $connectionString;

  }

  public function getServerFromShardType(TType $shardType): ConnectionDetails {
    $shardIndex = $this->getShardId($shardType);
    return $this->getServerByOffset($shardIndex);
  }
}
