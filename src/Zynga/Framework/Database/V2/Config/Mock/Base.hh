<?hh // strict

namespace Zynga\Framework\Database\V2\Config\Mock;

use Zynga\Framework\Database\V2\Config\Base as ConfigBase;

use Zynga\Framework\Database\V2\Driver\Mock as MockDriver;
use Zynga\Framework\Database\V2\Driver\Mock\Quoter as MockQuoter;
use Zynga\Framework\Database\V2\Driver\Mock\Transaction as MockTransaction;

use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;

abstract class Base extends ConfigBase {

  public function getDriver(): string {
    return 'Mock';
  }

  public function isDatabaseReadOnly(): bool {
    return false;
  }

  public function getConnectionString(): string {
    $connectionString = '';
    $connectionString .= 'username='.$this->getUserName();
    $connectionString .= 'password='.$this->getPassword();
    return $connectionString;
  }

}
