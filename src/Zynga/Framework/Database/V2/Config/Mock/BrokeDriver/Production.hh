<?hh // strict

namespace Zynga\Framework\Database\V2\Config\Mock\BrokeDriver;

use Zynga\Framework\Database\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;

use Zynga\Framework\Database\V2\Config\ClusterBase as ConfigClusterBase;
use Zynga\Framework\Database\V2\Driver\Mock as MockDriver;
use Zynga\Framework\Database\V2\Driver\Mock\Quoter as MockQuoter;
use Zynga\Framework\Database\V2\Driver\Mock\Transaction as MockTransaction;

class Production extends ConfigClusterBase {
  const int SERVER_ID_START = 1;
  const int SERVER_ID_STOP = 5;
  const string SERVER_USERNAME = 'someuser';
  const string SERVER_PASSWORD = 'somepass';
  const int SERVER_PORT = 12345;

  public function isDatabaseReadOnly(): bool {
    return false;
  }

  public function clusterInit(): bool {

    for (
      $serverId = self::SERVER_ID_START;
      $serverId <= self::SERVER_ID_STOP;
      $serverId++
    ) {

      $serverName = sprintf('poker-dummy-%02d', $serverId);

      $this->addServer($serverName);

    }

    $this->setUserName(self::SERVER_USERNAME);

    $this->setPassword(self::SERVER_PASSWORD);

    $this->setPort(self::SERVER_PORT);

    return true;

  }

  public function getDriver(): string {
    return 'NoSuchDriver';
  }

  /**
   * Gets the connection specific string used for the driver.
   * @type string
   */
  public function getConnectionString(): string {
    $connectionString = '';
    $connectionString .= 'username='.$this->getUserName().';';
    $connectionString .= 'password='.$this->getPassword().';';
    $connectionString .= 'host='.$this->getRandomServer().';';
    return $connectionString;
  }

}
