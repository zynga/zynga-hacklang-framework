<?hh // strict

namespace Zynga\Framework\PgData\V1\Testing\Mock;

use Zynga\Framework\PgData\V1\Exceptions\UnsupportedOperandException;
use Zynga\Framework\PgData\V1\Test\ExampleFeature\Model\Inventory\ItemType;
use Zynga\Framework\PgData\V1\Testing\Mock\PgModel as MockPgModel;
use Zynga\Framework\PgData\V1\Testing\Mock\PgReaderWriter;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\PgData\V1\PgWhereClause;
use Zynga\Framework\PgData\V1\PgWhereOperand;
use Zynga\Framework\Type\V1\UInt64Box;

class PgUnitTestReaderWriterTest extends TestCase {

  public function testAddDelete(): void {
    $model = new MockPgModel();
    $reader = $model->reader();
    $writer = $model->writer();

    // Define an item.
    $item = new ItemType($model);
    $item->id->set(1);
    $item->name->set("cool_name");

    // Ensure the item isn't present.
    $this->assertNull(
      $reader->getByPk(get_class($item), $item->id->get(), false),
    );

    // Add an item.
    $writer->add($item, false);

    // Ensure the item is present.
    $this->assertNotNull(
      $reader->getByPk(get_class($item), $item->id->get(), false),
    );

    // Delete the item.
    $item->delete();

    // Ensure the item isn't present.
    $this->assertNull(
      $reader->getByPk(get_class($item), $item->id->get(), false),
    );
  }

  public function testInThrowsException(): void {
    $model = new MockPgModel();
    $rw = $model->getMockReaderWriter();

    // Define an item.
    $item = new ItemType($model);
    $item->id->set(1);
    $item->name->set("cool_name");

    // Add an item.
    $rw->add($item, false);

    // Query with an IN operand.
    $this->expectException(UnsupportedOperandException::class);
    $where = new PgWhereClause($model);
    $where->and('id', PgWhereOperand::IN, $item->id->get());
    $this->assertEquals(1, $rw->get(get_class($item), $where)->count());
  }

  public function testOrThrowsException(): void {
    $model = new MockPgModel();
    $rw = $model->getMockReaderWriter();

    // Define an item.
    $item = new ItemType($model);
    $item->id->set(1);
    $item->name->set("cool_name");

    // Add an item.
    $rw->add($item, false);

    // Query with an OR pragma.
    $this->expectException(UnsupportedOperandException::class);
    $where = new PgWhereClause($model);
    $where->or('id', PgWhereOperand::EQUALS, $item->id->get());
    $this->assertEquals(1, $rw->get(get_class($item), $where)->count());
  }

  public function testWhere(): void {
    $model = new MockPgModel();
    $rw = $model->getMockReaderWriter();

    // Define an item.
    $item = new ItemType($model);
    $item->id->set(1);
    $item->name->set("cool_name");

    // Add an item.
    $rw->add($item, false);

    // Ensure an item is present.
    // (null, implicit where)
    $this->assertEquals(1, $rw->get(get_class($item))->count());

    // Use other operands too.
    $where = new PgWhereClause($model);
    $where->and('id', PgWhereOperand::NOT_EQUALS, 2);
    $where->and('id', PgWhereOperand::LESS_THAN, 2);
    $where->and('id', PgWhereOperand::GREATER_THAN, 0);
    $this->assertEquals(1, $rw->get(get_class($item), $where)->count());
  }

  public function testSavingRow(): void {
    $model = new MockPgModel();
    $rw = $model->getMockReaderWriter();

    $item = new ItemType($model);
    $item->id->set(1);
    $item->name->set("cool_name");

    $this->assertTrue($rw->save($item));
    $this->assertEquals(
      $item,
      $rw->getByPk(ItemType::class, $item->id, false),
    );

    $item2 = new ItemType($model);
    $item2->id->set(2);
    $item2->name->set("cool_name_2");

    $rw->saveSucceeds = false;
    $this->assertFalse($rw->save($item2));
    $this->assertNull($rw->getByPk(ItemType::class, $item2->id, false));
  }

  public function testAddingRow(): void {
    $model = new MockPgModel();
    $rw = $model->getMockReaderWriter();

    $item = new ItemType($model);
    $item->name->set("cool_name");

    $this->assertTrue($rw->add($item));
    $this->assertEquals(
      $item,
      $rw->getByPk(ItemType::class, new UInt64Box(1), false),
    );

    $this->assertEquals(1, $item->id->get());

    $item2 = new ItemType($model);
    $item2->name->set("cool_name");

    $rw->addSucceeds = false;
    $this->assertFalse($rw->add($item2));
    $this->assertNull($rw->getByPk(ItemType::class, new UInt64Box(2), false));
  }

  public function testGettingRowsWithEqualsOperator(): void {
    $model = new MockPgModel();
    $rw = $model->getMockReaderWriter();

    $item = new ItemType($model);
    $item->id->set(1);
    $item->name->set("cool_name");

    $item2 = new ItemType($model);
    $item2->id->set(4);
    $item2->name->set("cool_name_2");

    $rw->add($item);
    $rw->add($item2);

    $where = new PgWhereClause($model);
    $where->and("id", PgWhereOperand::EQUALS, 4);
    $resultSet = $rw->get(ItemType::class, $where);

    $this->assertEquals(1, $resultSet->count());
    $this->assertEquals($item2, $resultSet->at(0));
  }

}
