<?hh // strict

namespace Zynga\Framework\ShardedDatabase\V3\Interfaces;

use Zynga\Framework\Factory\V2\Interfaces\ConfigInterface;
use Zynga\Framework\ShardedDatabase\V3\ConnectionDetails;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

/**
 * DriverConfigInterface: Responsible for the configuration contract for all database drivers.
 */
interface DriverConfigInterface<TType as TypeInterface>
  extends ConfigInterface {

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
  public function getConnectionString(TType $shardType): string;

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
  public function getShardId(TType $shardType): int;

  /**
   * For sharded databases return the string hostname for your shard
   * @param int $sn
   * @param int $uid
   * @return string
   */
  public function getServerFromShardType(TType $shardType): ConnectionDetails;

  public function getShardCount(): int;

  public function getServerByOffset(int $offset): ConnectionDetails;

  public function getConnectionStringForServer(
    TType $shardType,
    ConnectionDetails $server,
  ): string;

  public function getDatabaseName(): string;

}
