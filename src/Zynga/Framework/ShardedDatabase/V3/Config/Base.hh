<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config;

use Zynga\Framework\Database\V2\Exceptions\NoPasswordException;
use Zynga\Framework\Database\V2\Exceptions\NoPortProvidedException;
use Zynga\Framework\Database\V2\Exceptions\NoUserNameException;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Factory\V2\Config\Base as FactoryBaseConfig;
use Zynga\Framework\ShardedDatabase\V3\Exceptions\InvalidShardIdException;
use Zynga\Framework\ShardedDatabase\V3\Exceptions\ShardsInitFailureException;
use
  Zynga\Framework\ShardedDatabase\V3\Exceptions\ShardsInitNoServersException
;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverConfigInterface;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\UInt64Box;
use Zynga\Framework\ShardedDatabase\V3\Exceptions\UnknownShardTypeException;

abstract class Base<TType as TypeInterface> extends FactoryBaseConfig
  implements DriverConfigInterface<TType> {

  private string $_currentServer;
  private string $_currentDatabase;
  private Vector<ConnectionDetails> $_servers = Vector {};

  final public function __construct() {
    $this->_currentServer = '';
    $this->_currentDatabase = '';
    $this->init();
  }

  abstract public function getDatabaseName(): string;

  /**
   * Initializes the configuration, and verifies that all the functions have been called.
   * @return bool
   */
  final public function init(): bool {
    $this->setCurrentDatabase($this->getDatabaseName());

    // reset our servers list to empty.
    $this->_servers->clear();

    try {
      if ($this->shardsInit() !== true) {
        throw new ShardsInitFailureException('Implementation returned false');
      }
    } catch (Exception $e) {
      throw $e;
    }

    if ($this->getServerCount() == 0) {
      throw new ShardsInitNoServersException(
        'You failed to add any servers to the list, call addServer',
      );
    }

    foreach ($this->_servers as $shardId => $server) {
      if ($server->getUsername() === '') {
        throw new NoUserNameException(
          sprintf(
            'NO_USERNAME at shard %d, verify shardsInit() specifies a username for each server object.',
            $shardId,
          ),
        );
      }

      if ($server->getPassword() === '') {
        throw new NoPasswordException(
          sprintf(
            'NO_PASSWORD at shard %d, verify shardsInit() specifies a password for each server object.',
            $shardId,
          ),
        );
      }

      if ($server->getPort() === 0) {
        throw new NoPortProvidedException(
          sprintf(
            'NO_PORT at shard %d, verify shardsInit() specifies a port for each server object.',
            $shardId,
          ),
        );
      }
    }

    return true;
  }

  final public function getCurrentServer(): string {
    return $this->_currentServer;
  }

  final public function setCurrentServer(string $server): bool {
    $this->_currentServer = $server;
    return true;
  }

  final public function getCurrentDatabase(): string {
    return $this->_currentDatabase;
  }

  final public function setCurrentDatabase(string $dbname): bool {
    $this->_currentDatabase = $dbname;
    return true;
  }

  /**
   * Gets the connection specific string used for the driver.
   * @param int Social network id, if the driver is supporting sharding by snid
   * @param int User id, if the driver is supporting sharding by uid
   * @return string
   */
  final public function getConnectionString(TType $shardType): string {
    $server = $this->getServerFromShardType($shardType);
    return $this->getConnectionStringForServer($shardType, $server);
  }

  final public function clearServers(): bool {
    $this->_servers->clear();
    return true;
  }

  final public function addServer(ConnectionDetails $server): bool {
    $this->_servers->add($server);
    return true;
  }

  final public function getServerCount(): int {
    return $this->_servers->count();
  }

  final public function getServerByOffset(int $offset): ConnectionDetails {
    if ($this->_servers->containsKey($offset) === true) {
      return $this->_servers[$offset];
    }
    throw new InvalidShardIdException('offset='.$offset);
  }

  public function getShardId(TType $shardType): int {
    $shardCount = $this->getServerCount();
    if ($shardType instanceof UInt64Box) {
      $shardId = $shardType->get() % $shardCount;
      return $shardId;
    } else if($shardType instanceof StringBox) {
      $crcString = crc32($shardType->get());
      $shardId = $crcString % $shardCount;
      return $shardId;
    }

    throw new UnknownShardTypeException("Unsupported TType");
  }

  /**
   * User definable and overloadable hook for initializing your cluster.
   * @return bool
   */
  abstract public function shardsInit(): bool;

  abstract public function getServerFromShardType(
    TType $shardType,
  ): ConnectionDetails;

  final public function getShardCount(): int {
    return $this->_servers->count();
  }

  abstract public function getConnectionStringForServer(
    TType $shardType,
    ConnectionDetails $server,
  ): string;

}
