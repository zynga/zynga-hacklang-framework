<?hh // strict

namespace Zynga\Framework\Factory\V2\Interfaces;

use Zynga\Framework\Factory\V2\Interfaces\ConfigInterface;
use Zynga\Framework\Factory\V2\Interfaces\DriverInterface;
use Zynga\Framework\Factory\V2\Interfaces\FactoryTemplateInterface;

interface FactoryInterface {

  /**
   * Defines the namespaced class path to the for this factory.
   * Ex: \Zynga\Database\V1
   * @return string
   */
  public static function getClassRoot(): string;

  public static function addClassRoot(string $root): bool;

  public static function factory<TDriver>(
    classname<TDriver> $driverName,
    string $name,
  ): TDriver;

  public static function clear(): bool;

  public static function getUseMockDrivers(): bool;

  public static function enableMockDrivers(): bool;

  public static function disableMockDrivers(): bool;

}
