<?hh // strict

namespace Zynga\Framework\Logging\V1;

use Zynga\Framework\Factory\V2\Base as FactoryBase;
use Zynga\Framework\Factory\V2\Interfaces\DriverInterface;

class Factory extends FactoryBase {

  public static function getClassRoot(): string {
    return 'Zynga\Framework\Logging\V1';
  }

  public static function factory<TDriver as DriverInterface>(
    classname<TDriver> $driverName,
    string $name,
  ): TDriver {
    if ($name == 'default') {
      $name = 'GlobalConfig';
    }
    return parent::factory($driverName, $name);
  }

}
