<?hh // strict

namespace Zynga\Framework\Database\V2\Config\Mock;

use Zynga\Framework\Database\V2\Config\ClusterBase as ClusterBaseClass;

use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;

use Zynga\Framework\Database\V2\Driver\Mock\Quoter as MockQuoter;
use Zynga\Framework\Database\V2\Driver\Mock\Transaction as MockTranscation;

abstract class ClusterBase extends ClusterBaseClass {
  const string TEST_USERNAME = 'testusername';
  const string TEST_PASSWORD = 'testpassword';
  const string TEST_SCHEMA = 'someschema';
  const int TEST_PORT = 12346;

  public function isDatabaseReadOnly(): bool {
    return false;
  }

  public function clusterInit(): bool {
    $this->setPort(self::TEST_PORT);
    $this->setUserName(self::TEST_USERNAME);
    $this->setPassword(self::TEST_PASSWORD);
    $this->setSchema(self::TEST_SCHEMA);

    return true;
  }

  public function getDriver(): string {
    return 'Mock';
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
