<?hh // strict

namespace Zynga\Framework\Testing\MockState\V2\Mocks;

class FactoryExample {
  private static bool $_mocks = false;

  public static function clear(): bool {
    self::$_mocks = false;
    return true;
  }
  
  public static function getUseMockDrivers(): bool {
    return self::$_mocks;
  }

  public static function disableMockDrivers(): bool {
    self::$_mocks = false;
    return true;
  }

  public static function enableMockDrivers(): bool {
    self::$_mocks = true;
    return true;
  }

}
