<?hh // strict

namespace Zynga\Framework\Database\V2\Config\Mock\Cluster\Broken;

use Zynga\Framework\Database\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;

use Zynga\Framework\Database\V2\Config\Mock\ClusterBase as ConfigClusterBase;
use Zynga\Framework\Database\V2\Driver\Mock as MockDriver;
use Zynga\Framework\Database\V2\Driver\Mock\Quoter as MockQuoter;
use Zynga\Framework\Database\V2\Driver\Mock\Transaction as MockTransaction;

class Dev extends ConfigClusterBase {

  public function clusterInit(): bool {

    parent::clusterInit();

    return false;

  }

}
