<?hh // strict

namespace Zynga\Framework\PgData\V1\PgModel;

use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Interfaces\QueryableInterface;
use
  Zynga\Framework\Database\V2\Interfaces\DriverInterface as DatabaseDriverInterface
;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\PgData\V1\Exceptions\UnsupportedValueTypeException;
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

  public function getReadDatabase(): QueryableInterface {
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

  public function getWriteDatabase(): QueryableInterface {
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

  public function quoteValue(QueryableInterface $dbh, mixed $value): string {

    if (is_string($value)) {
      return $dbh->quote()->textValue($value);
    } else if (is_float($value)) {
      return $dbh->quote()->floatValue($value);
    } else if (is_int($value)) {
      return $dbh->quote()->intValue($value);
    }

    throw new UnsupportedValueTypeException('value='.gettype($value));

  }

}
