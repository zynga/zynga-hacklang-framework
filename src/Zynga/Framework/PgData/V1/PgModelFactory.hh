<?hh //strict

namespace Zynga\Framework\PgData\V1;

use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;

abstract class PgModelFactory {
  private static ?PgModelInterface $_mock;

  public function getModel(): PgModelInterface {
    if (self::$_mock !== null) {
      return self::$_mock;
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
