<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Config;

use Zynga\Framework\Database\V2\Exceptions\NoPasswordException;
use Zynga\Framework\Database\V2\Exceptions\NoPortProvidedException;
use Zynga\Framework\Database\V2\Exceptions\NoUserNameException;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Factory\V2\Config\Base as FactoryBaseConfig;

use
  Zynga\Framework\ShardedDatabase\V3\Exceptions\InvalidShardIdException
;
use
  Zynga\Framework\ShardedDatabase\V3\Exceptions\ShardsInitFailureException
;
use
  Zynga\Framework\ShardedDatabase\V3\Exceptions\ShardsInitNoServersException
;
use
  Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverConfigInterface
;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;

abstract class Base extends FactoryBaseConfig
  implements DriverConfigInterface {
  private string $_currentServer;
  private string $_currentDatabase;
  private Vector<ConnectionDetails> $_servers = Vector {};

  final public function __construct() {
    $this->_currentServer = '';
    $this->_currentDatabase = '';
    $this->init();
  }

  /**
   * Initializes the configuration, and verifies that all the functions have been called.
   * @return bool
   */
  final public function init(): bool {
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
  final public function getConnectionString(SnidBox $sn, UidBox $uid): string {
    $server = $this->getServerFromUserId($sn, $uid);
    return $this->getConnectionStringForServer($sn, $server);
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

  /**
   * User definable and overloadable hook for initializing your cluster.
   * @return bool
   */
  abstract public function shardsInit(): bool;

  abstract public function getShardId(SnidBox $sn, UidBox $uid): int;

  abstract public function getServerFromUserId(
    SnidBox $sn,
    UidBox $uid,
  ): ConnectionDetails;

  abstract public function getShardCount(): int;

  abstract public function getConnectionStringForServer(
    SnidBox $sn,
    ConnectionDetails $server,
  ): string;
}
