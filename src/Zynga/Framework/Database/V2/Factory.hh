<?hh // strict

namespace Zynga\Framework\Database\V2;

use Zynga\Framework\Factory\V2\Base as FactoryBase;

use
  Zynga\Framework\Database\V2\Interfaces\DriverInterface as DatabaseDriverInterface
;
use Zynga\Framework\Database\V2\Driver\Mock as MockDatabaseDriver;
use Zynga\Framework\Exception\V1\Exception;

class Factory extends FactoryBase {

  public static function getClassRoot(): string {
    return '\Zynga\Framework\Database\V2';
  }

  public static function loadResultsForTest(
    string $class,
    string $function,
  ): bool {
    try {
      $dbh = self::factory(MockDatabaseDriver::class, 'Mock');
      return $dbh->loadResultsForTest($class, $function);
    } catch (Exception $e) {
      throw $e;
    }

  }

}
