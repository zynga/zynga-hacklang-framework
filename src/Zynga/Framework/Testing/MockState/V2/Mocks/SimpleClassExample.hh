<?hh // strict

namespace Zynga\Framework\Testing\MockState\V2\Mocks;

class SimpleClassExample {
  private static bool $_mocks = false;

  public static function clear(): bool {
    self::$_mocks = false;
    return true;
  }

  public static function getMockState(): bool {
    return self::$_mocks;
  }

  public static function disableMocks(): bool {
    self::$_mocks = false;
    return true;
  }

  public static function enableMocks(): bool {
    self::$_mocks = true;
    return true;
  }

}
