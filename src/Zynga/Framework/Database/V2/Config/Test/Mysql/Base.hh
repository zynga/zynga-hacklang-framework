<?hh // strict

namespace Zynga\Framework\Database\V2\Config\Test\Mysql;

use Zynga\Framework\Database\V2\Config\Base as ConfigBase;

/**
 * Base implementation of a config for a Game DB
 */
abstract class Base extends ConfigBase {

  const int Port = 3306;
  const string UserName = 'zframework';
  const string Password = 'i-am-a-walrus';
  const string Schema = 'phpunit';

  /**
   * Initialize this config
   *
   * @return True if successful, otherwise false
   */
  public function init(): bool {
    $this->setUserName(self::UserName);
    $this->setPassword(self::Password);
    $this->setPort(self::Port);
    $this->setCurrentServer($this->getInitialHostName());
    $this->setSchema(self::Schema);

    // We have to call parent::init last as it verifies that the base level settings are done.
    return parent::init();
  }

  public function isDatabaseReadOnly(): bool {
    return false;
  }

  /**
   * Get the host to initially use when initializing this config
   *
   * @return Host to initially use when initializing this config
   */
  public function getInitialHostName(): string {
    return 'localhost';
  }

  public function getDriver(): string {
    return 'GenericPDO\Base';
  }

  /**
   * Get a PDO formatted string for this config
   *
   * @return PDO formatted string for this config
   */
  public function getConnectionString(): string {
    $connectionString = 'mysql:';
    $connectionString .= 'host='.$this->getCurrentServer().';';
    $connectionString .= 'user='.$this->getUserName().';';
    $connectionString .= 'password='.$this->getPassword().';';
    $connectionString .= 'port='.$this->getPort().';';
    $connectionString .= 'dbname='.$this->getSchema().';';
    return $connectionString;
  }

}
