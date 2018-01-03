<?hh // strict

namespace Zynga\Framework\Dynamic\V1\Mocks;

class StaticClass {

  public static function testFunction(): bool {
    return true;
  }

  public static function testFunctionWithParams(
    string $param1,
    int $param2,
  ): string {
    return $param1.":".$param2;
  }

}
