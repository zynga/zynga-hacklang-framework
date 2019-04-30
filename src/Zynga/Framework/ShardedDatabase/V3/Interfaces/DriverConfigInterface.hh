<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Interfaces;

use Zynga\Framework\Factory\V2\Interfaces\ConfigInterface;

use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;

/**
 * DriverConfigInterface: Responsible for the configuration contract for all database drivers.
 */
interface DriverConfigInterface extends ConfigInterface {

  /**
   * Initializes the configuration, and verifies that all the functions have been called.
   * @return bool
   */
  public function init(): bool;

  /**
   * The database is a read only database and should toss errors if you attempt
   *  DML with this connections.
   * @return bool
   */
  public function isDatabaseReadOnly(): bool;

  /**
   * Gets the connection specific string used for the driver.
   * @type string
   */
  public function getConnectionString(SnidBox $sn, UidBox $uid): string;

  /**
   * Gets the current server you are connected to.
   * @return string
   */
  public function getCurrentServer(): string;

  /**
   * Set the current server.
   * @param string
   */
  public function setCurrentServer(string $server): bool;

  /**
   * Gets the current database you are connected to.
   * @return string
   */
  public function getCurrentDatabase(): string;

  /**
   * Sets the current database to the requested name.
   * @param string
   * @return bool
   */
  public function setCurrentDatabase(string $dbname): bool;

  /**
   * For sharded databases return the int id that your shard algorithim generates.
   * @param int $sn
   * @param int $uid
   * @return int
   */
  public function getShardId(SnidBox $sn, UidBox $uid): int;

  /**
   * For sharded databases return the string hostname for your shard
   * @param int $sn
   * @param int $uid
   * @return string
   */
  public function getServerFromUserId(SnidBox $sn, UidBox $uid): ConnectionDetails;

  public function getShardCount(): int;

  public function getServerByOffset(int $offset): ConnectionDetails;

  public function getConnectionStringForServer(
    SnidBox $sn,
    ConnectionDetails $server,
  ): string;
}
