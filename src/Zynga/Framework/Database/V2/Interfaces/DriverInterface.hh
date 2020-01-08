<?hh // strict

namespace Zynga\Framework\Database\V2\Interfaces;

use Zynga\Framework\Database\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Database\V2\Interfaces\ResultSetInterface;

use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;
use
  Zynga\Framework\Factory\V2\Interfaces\DriverInterface as FactoryDriverInterface
;

interface DriverInterface extends FactoryDriverInterface, QueryableInterface {

  public function getRequiresMockQueries(): bool;
  public function enableRequireMockQueries(): bool;
  public function disableRequireMockQueries(): bool;

  public function getConfig(): DriverConfigInterface;

}
