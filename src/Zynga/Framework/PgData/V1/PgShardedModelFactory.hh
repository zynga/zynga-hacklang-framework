<?hh //strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\PgData\V1\Interfaces\Sharded\PgModelInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\PgData\V1\Testing\Mock\PgShardedModel as MockPgShardedModel;

abstract class PgShardedModelFactory {
  public static bool $overrideShardKey = false;
  private static ?PgModelInterface $_mock;

  public function __construct(private TypeInterface $_shardKey) {}

  public function getModel(): PgModelInterface {
    if (self::$_mock !== null) {
      $mockedClass = self::$_mock;
      if(self::$overrideShardKey && ($mockedClass instanceof MockPgShardedModel)) {
        $mockedClass->setShardId($this->_shardKey);
      }

      return $mockedClass;
    }
    return $this->getRealModel();
  }

  public static function enableMock(PgModelInterface $mock): void {
    self::$_mock = $mock;
  }

  public static function disableMock(): void {
    self::$_mock = null;
  }

  protected abstract function getRealModel(): PgModelInterface;

}
