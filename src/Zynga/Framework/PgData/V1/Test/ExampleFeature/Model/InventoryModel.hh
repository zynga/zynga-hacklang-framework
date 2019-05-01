<?hh // strict

namespace Zynga\Framework\PgData\V1\Test\ExampleFeature\Model;

use Zynga\Framework\PgData\V1\PgModel;

class InventoryModel extends PgModel {

  public function getReadDatabaseName(): string {
    return 'Test_Mysql';
  }

  public function getWriteDatabaseName(): string {
    return 'Test_Mysql';
  }

}
