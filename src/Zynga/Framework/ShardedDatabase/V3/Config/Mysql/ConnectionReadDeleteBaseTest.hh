<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Mysql;

use Zynga\Framework\Database\V2\Exceptions\ConnectionIsReadOnly;
use Zynga\Framework\Factory\V2\Test\MockState as FactoryMockState;

use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use
  Zynga\Framework\ShardedDatabase\V3\Config\Mysql\ConnectionBaseTest
;

abstract class ConnectionReadDeleteBaseTest extends ConnectionBaseTest {

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
      $this->getDriverName(),
    );

    $randSN = $this->getRandomSocialNetwork();
    $randUID = $this->getRandomTestUserId();
    $dbh->setSnUid($randSN, $randUID);

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
      $this->getDriverName(),
    );

    $sql =
      'DELETE FROM '.
      $this->getTableNameForTests().
      ' WHERE unit_test_stamp = 0 ';

    $testSn = $this->getRandomSocialNetwork();
    $testUid = $this->getRandomTestUserId();

    $dbh->setSnUid($testSn, $testUid);

    $delSth = $dbh->query($sql);

    $this->assertTrue($delSth->wasSuccessful());

    $this->assertEquals(0, $delSth->getNumRows());

    $dbh->transaction()->commit();

  }

}
