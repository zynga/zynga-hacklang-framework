<?hh // strict

namespace Zynga\Framework\Database\V2\Config;

use Zynga\Framework\Database\V2\Exceptions\NoUserNameException;
use Zynga\Framework\Database\V2\Exceptions\NoPasswordException;
use Zynga\Framework\Database\V2\Exceptions\NoPortProvidedException;
use Zynga\Framework\Database\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Factory\V2\Config\Base as FactoryBaseConfig;

abstract class Base extends FactoryBaseConfig implements DriverConfigInterface {

  private string $_currentServer;
  private string $_currentDatabase;
  private string $_username;
  private string $_password;
  private string $_schema;
  private int $_port;

  final public function __construct() {
    $this->_currentServer = '';
    $this->_currentDatabase = '';
    $this->_username = '';
    $this->_password = '';
    $this->_schema = '';
    $this->_port = 0;
    $this->init();
  }

  /**
   * Sets the user name to be utilized for the driver.
   * @param string $userName
   * @return bool success
   */
  final public function setUserName(string $username): bool {
    $this->_username = $username;
    return true;
  }

  /**
   * Gets the username that is currently configured for this driver.
   * @return string user_name
   */
  final public function getUserName(): string {
    return $this->_username;
  }

  /**
   * Sets the password to be utilized for this driver.
   * @param string $password
   */
  final public function setPassword(string $password): bool {
    $this->_password = $password;
    return true;
  }

  /**
   * Gets the password that is currently configured for this driver.
   * @return string
   */
  final public function getPassword(): string {
    return $this->_password;
  }

  /**
   * Sets the port to be utilized for this driver.
   * @param int $port
   * @return bool
   */
  final public function setPort(int $port): bool {
    $this->_port = $port;
    return true;
  }

  /**
   * Get the port that is currently configured for this driver.
   * @return int
   */
  final public function getPort(): int {
    return $this->_port;
  }

  /**
   * Initializes the configuration, and verifies that all the functions have been called.
   * @return bool
   */
  public function init(): bool {

    if ($this->_username === '') {
      throw new NoUserNameException(
        'NO_USERNAME, call setUserName within your init function',
      );
    }

    if ($this->_password === '') {
      throw new NoPasswordException(
        'NO_PASSWORD, call setPassword within your init function',
      );
    }

    if ($this->_port === 0) {
      throw new NoPortProvidedException(
        'NO_PORT, call setPort within your init function',
      );
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

  final public function getSchema(): string {
    return $this->_schema;
  }

  final public function setSchema(string $schema): bool {
    $this->_schema = $schema;
    return true;
  }

  /**
   * Gets the connection specific string used for the driver.
   * @param int Social network id, if the driver is supporting sharding by snid
   * @param int User id, if the driver is supporting sharding by uid
   * @return string
   */
  abstract public function getConnectionString(): string;

}
