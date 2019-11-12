<?hh //strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

abstract class PgShardedModelFactory {
  private static ?PgModelInterface $_mock;

  public static function getModel(TypeInterface $shardKey): PgModelInterface {
    if (self::$_mock !== null) {
      return self::$_mock;
    }
    return static::getRealModel($shardKey);
  }
  public static function enableMock(PgModelInterface $mock): void {
    self::$_mock = $mock;
  }

  public static function disableMock(): void {
    self::$_mock = null;
  }

  protected abstract static function getRealModel(
    TypeInterface $shardKey,
  ): PgModelInterface;

}
