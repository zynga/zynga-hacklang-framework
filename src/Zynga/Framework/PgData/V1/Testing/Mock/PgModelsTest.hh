<?hh //strict

namespace Zynga\Framework\PgData\V1\Testing\Mock;

use Zynga\Framework\PgData\V1\Testing\Mock\PgModel as MockPgModel;
use
  Zynga\Framework\PgData\V1\Testing\Mock\PgShardedModel as MockPgModelShard
;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\UInt64Box;

class PgModelsTest extends TestCase {
  public function testPgModelMocksDriverNames(): void {
    $model = new MockPgModel();
    $this->assertEquals('Mock\PgData', $model->getDataCacheName());
    $this->assertEquals('Mock\PgResultSet', $model->getResultSetCacheName());
    $this->assertEquals('Mock', $model->getReadDatabaseName());
    $this->assertEquals('Mock', $model->getWriteDatabaseName());
  }

  public function testShardedPgModelMocksDriverNames(): void {
    $id = new UInt64Box(4);
    $model = new MockPgModelShard($id);
    $this->assertEquals('Mock\PgData', $model->getDataCacheName());
    $this->assertEquals('Mock\PgResultSet', $model->getResultSetCacheName());
    $this->assertEquals('Mock', $model->getReadDatabaseName());
    $this->assertEquals('Mock', $model->getWriteDatabaseName());
    $this->assertEquals($id->get(), $model->getShardId()->get());
  }
}
