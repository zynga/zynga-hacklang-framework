<?hh // strict

namespace Zynga\Framework\PgData\V1\PgModel;

use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use
  Zynga\Framework\Database\V2\Interfaces\DriverInterface as DatabaseDriverInterface
;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\DbInterface;
use Zynga\Framework\PgData\V1\Interfaces\PgModelInterface;

class Db implements DbInterface {

  private PgModelInterface $_pgModel;

  public function __construct(PgModelInterface $pgModel) {
    $this->_pgModel = $pgModel;
  }

  private function pgModel(): PgModelInterface {
    return $this->_pgModel;
  }

  public function getReadDatabase(): DatabaseDriverInterface {
    try {

      $pgModel = $this->pgModel();
      return DatabaseFactory::factory(
        DatabaseDriverInterface::class,
        $pgModel->getReadDatabaseName(),
      );
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function getWriteDatabase(): DatabaseDriverInterface {
    try {
      $pgModel = $this->pgModel();
      return DatabaseFactory::factory(
        DatabaseDriverInterface::class,
        $pgModel->getWriteDatabaseName(),
      );
    } catch (Exception $e) {
      throw $e;
    }
  }

}
