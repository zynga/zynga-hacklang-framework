<?hh //strict

namespace Zynga\Framework\PgData\V1\Test\ExampleFeature\Model;

use
  Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\Sharded\ShardedInventoryFactory
;
use
  Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\Sharded\ShardedInventoryModel
;
use Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\InventoryModel;
use
  Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\InventoryModelFactory
;
use Zynga\Framework\PgData\V1\Testing\Mock\PgModel as MockPgModel;
use
  Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\Sharded\ShardedInventoryModel as MockPgShardedModel
;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\UInt64Box;

class FactoryTest extends TestCase {

  public function setUp(): void {
    parent::setUp();
    ShardedInventoryFactory::disableMock();
    InventoryModelFactory::disableMock();
  }

  public function tearDown(): void {
    ShardedInventoryFactory::disableMock();
    InventoryModelFactory::disableMock();
    parent::tearDown();
  }

  public function testSettingMockReturnsSameInstanceAsTheSetMock(): void {
    $mockShardedModel = new MockPgShardedModel(new UInt64Box());
    $mockModel = new MockPgModel();
    ShardedInventoryFactory::enableMock($mockShardedModel);
    InventoryModelFactory::enableMock($mockModel);

    $sharedFactory = new ShardedInventoryFactory(new UInt64Box());

    $this->assertSame($mockShardedModel, $sharedFactory->getModel());

    $this->assertSame($mockModel, InventoryModelFactory::getModel());
  }

  public function testGettingModelsWithoutMockReturnsActaulInstancesOfTheModels(
  ): void {
    $sharedFactory = new ShardedInventoryFactory(new UInt64Box());

    $this->assertInstanceOf(
      ShardedInventoryModel::class,
      $sharedFactory->getModel(),
    );
    $this->assertInstanceOf(
      InventoryModel::class,
      InventoryModelFactory::getModel(),
    );
  }
}
