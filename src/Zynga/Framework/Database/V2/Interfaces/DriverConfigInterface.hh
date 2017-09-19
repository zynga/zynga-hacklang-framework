<?hh // strict

namespace Zynga\Framework\Database\V2\Interfaces;

use Zynga\Framework\Factory\V2\Interfaces\ConfigInterface;

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
   * Sets the user name to be utilized for the driver.
   * @param string $userName
   * @return bool success
   */
  public function setUserName(string $userName): bool;

  /**
   * Gets the username that is currently configured for this driver.
   * @return string user_name
   */
  public function getUserName(): string;

  /**
   * Sets the password to be utilized for this driver.
   * @param string $password
   */
  public function setPassword(string $password): bool;

  /**
   * Gets the password that is currently configured for this driver.
   * @return string
   */
  public function getPassword(): string;

  /**
   * Gets the connection specific string used for the driver.
   * @type string
   */
  public function getConnectionString(): string;

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

}
