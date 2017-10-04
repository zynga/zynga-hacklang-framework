<?hh // strict

namespace Zynga\Framework\Database\V2\Config\Mock\Cluster;

use Zynga\Framework\Database\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;

use Zynga\Framework\Database\V2\Config\Mock\ClusterBase as ConfigClusterBase;
use Zynga\Framework\Database\V2\Driver\Mock as MockDriver;
use Zynga\Framework\Database\V2\Driver\Mock\Quoter as MockQuoter;
use Zynga\Framework\Database\V2\Driver\Mock\Transaction as MockTransaction;

class Dev extends ConfigClusterBase {
  const int SERVER_ID_START = 1;
  const int SERVER_ID_STOP = 5;

  public function isDatabaseReadOnly(): bool {
    return false;
  }

  public function clusterInit(): bool {

    parent::clusterInit();

    $this->clearServers();

    for (
      $serverId = self::SERVER_ID_START;
      $serverId <= self::SERVER_ID_STOP;
      $serverId++
    ) {

      $serverName = sprintf('poker-mock-%02d', $serverId);

      $this->addServer($serverName);

    }

    return true;

  }

}
