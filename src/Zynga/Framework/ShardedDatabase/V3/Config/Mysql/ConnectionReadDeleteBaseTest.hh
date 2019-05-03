<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Mysql;

use Zynga\Framework\Database\V2\Exceptions\ConnectionIsReadOnly;
use Zynga\Framework\Factory\V2\Test\MockState as FactoryMockState;

use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use
  Zynga\Framework\ShardedDatabase\V3\Config\Mysql\ConnectionBaseTest
;
use Zynga\Framework\Type\V1\UInt64Box;

abstract class ConnectionReadDeleteBaseTest extends ConnectionBaseTest<UInt64Box> {

  public function doSetUpBeforeClass(): bool {
    parent::doSetUpBeforeClass();
    DatabaseFactory::disableMockDrivers();
    return true;
  }

  <<__Override>>
  public function testValidInsertCommitSelectQuery(): void {
    $this->assertFalse(false);
  }

  <<__Override>>
  public function testValidInsertQuery(): void {
    $this->assertFalse(false);
  }

  <<__Override>>
  public function testValidInsertCommitQuery(): void {
    $this->assertFalse(false);
  }

  public function testValidDeleteQuery(): void {
    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getConfigName(),
    );

    $randShard = $this->getRandomShardType();
    $dbh->setShardType($randShard);

    $sql =
      'DELETE FROM '.
      $this->getTableNameForTests().
      ' WHERE unit_test_stamp = 0';

    $delSth = $dbh->query($sql);

    $this->assertTrue($delSth->hasCursor());
    $this->assertEquals(0, $delSth->getNumRows());
  }

  public function testValidDeleteCommitQuery(): void {

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getConfigName(),
    );

    $sql =
      'DELETE FROM '.
      $this->getTableNameForTests().
      ' WHERE unit_test_stamp = 0 ';

    $randShard = $this->getRandomShardType();
    $dbh->setShardType($randShard);

    $delSth = $dbh->query($sql);

    $this->assertTrue($delSth->wasSuccessful());

    $this->assertEquals(0, $delSth->getNumRows());

    $dbh->transaction()->commit();

  }

}
