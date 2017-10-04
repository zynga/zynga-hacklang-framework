<?hh // strict

namespace Zynga\Framework\Database\V2\Config\Mock;

use Zynga\Framework\Database\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;

use Zynga\Framework\Database\V2\Driver\Mock\Quoter as NativeQuoter;
use Zynga\Framework\Database\V2\Driver\Mock\Transaction as NativeTransaction;

use Zynga\Framework\Database\V2\Config\Mock\Base;

class Production extends Base {

  const string SERVER_USERNAME = 'mockuser';
  const string SERVER_PASSWORD = 'mockpass';
  const int SERVER_PORT = 1337;
  const string CURRENT_DATABASE = 'mockdb';
  const string CURRENT_SERVER = 'mockhost';

  public function init(): bool {

    $this->setCurrentDatabase(self::CURRENT_DATABASE);
    $this->setCurrentServer(self::CURRENT_SERVER);
    $this->setPort(self::SERVER_PORT);
    $this->setUserName(self::SERVER_USERNAME);
    $this->setPassword(self::SERVER_PASSWORD);

    return parent::init();

  }

}
