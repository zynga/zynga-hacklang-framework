<?hh // strict

namespace Zynga\Framework\PgData\V1\Sharded\PgModel;

use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Interfaces\QueryableInterface;
use
  Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface as DatabaseDriverInterface
;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\PgData\V1\Exceptions\UnsupportedValueTypeException;
use Zynga\Framework\PgData\V1\Interfaces\PgModel\DbInterface;
use Zynga\Framework\PgData\V1\Interfaces\Sharded\PgModelInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

class Db implements DbInterface {

  private PgModelInterface $_pgModel;
  private TypeInterface $_shardId;

  public function __construct(
    PgModelInterface $pgModel,
    TypeInterface $shardId,
  ) {
    $this->_pgModel = $pgModel;
    $this->_shardId = $shardId;
  }

  private function pgModel(): PgModelInterface {
    return $this->_pgModel;
  }

  private function shardId(): TypeInterface {
    return $this->_shardId;
  }

  public function getReadDatabase(): QueryableInterface {
    try {

      $pgModel = $this->pgModel();
      return DatabaseFactory::getDriver(
        $pgModel->getReadDatabaseName(),
        $this->shardId(),
      );
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function getWriteDatabase(): QueryableInterface {
    try {
      $pgModel = $this->pgModel();
      return DatabaseFactory::getDriver(
        $pgModel->getWriteDatabaseName(),
        $this->shardId(),
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
    } else if (is_bool($value)) {
      return $dbh->quote()->boolValue($value);
    }

    throw new UnsupportedValueTypeException('value='.gettype($value));

  }

}
