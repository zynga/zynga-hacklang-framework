<?hh // strict

namespace Zynga\Framework\Database\V2\Config\Test\Vertica;

use Zynga\Framework\Database\V2\Config\Base as ConfigBase;

/**
 * Base implementation of a config for a Game DB
 */
abstract class Base extends ConfigBase {

    const int Port = 5432;
    const string UserName = 'zframework';
    const string Password = 'i-am-a-walrus';
    const string Schema = 'phpunit';
    const int Connect_Timeout = 5;

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
      return 'Vertica';
    }


    /**
     * Gets the connection specific string used for the driver.
     * @type string
     */
    public function getConnectionString(): string {
      // Vertica doesn't care about sn
      //
      // --
      // Pick a server randomly, and then sticky to it for the life of this connection.
      // --
      if ($this->getCurrentServer() == '') {

        $server = $this->getInitialHostName();

        if (is_string($server)) {
          $this->setCurrentServer($server);
        }

      }

      $connectionString = '';
      $connectionString .= 'host='.$this->getCurrentServer().' ';
      $connectionString .= 'user='.$this->getUserName().' ';
      $connectionString .= 'password='.$this->getPassword().' ';
      $connectionString .= 'port='.$this->getPort().' ';
      $connectionString .= 'dbname='.$this->getSchema().' ';
      // So it turns out that vertica layer will literally wait forever, always send a timeout.
      $connectionString .= 'connect_timeout=' . self::Connect_Timeout;
      return $connectionString;
    }

}
