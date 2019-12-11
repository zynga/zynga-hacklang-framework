<?hh // strict

namespace Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\Sharded;

use Zynga\Framework\PgData\V1\Interfaces\Sharded\PgModelInterface;
use Zynga\Framework\PgData\V1\Sharded\PgModel;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;
use Zynga\Framework\Type\v1\UInt64Box;

class ShardedInventoryModel extends PgModel {

  public function getDataCacheName(): string {
    return 'PgDataTest';
  }

  public function getResultSetCacheName(): string {
    return 'PgResultSetTest';
  }

  public function getReadDatabaseName(): string {
    return 'Test_Mysql';
  }

  public function getWriteDatabaseName(): string {
    return 'Test_Mysql';
  }

}
