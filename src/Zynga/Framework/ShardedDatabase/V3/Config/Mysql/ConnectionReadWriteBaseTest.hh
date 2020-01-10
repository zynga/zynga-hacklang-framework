<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Mysql;

use Zynga\Framework\ShardedDatabase\V3\Config\Mysql\ConnectionBaseTest;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\Factory\V2\Test\MockState as FactoryMockState;
use Zynga\Framework\Type\V1\UInt64Box;
abstract class ConnectionReadWriteBaseTest
  extends ConnectionBaseTest<UInt64Box> {

  public function doSetUpBeforeClass(): bool {

    parent::doSetUpBeforeClass();

    DatabaseFactory::disableMockDrivers();

    return true;

  }

  public function testValidInsertCommitSelectQuery(): void {

    $dbh = $this->getDriverToTest();

    $randShard = $this->getRandomShardType();

    $ts = $this->getUnitTestStamp();

    $expectedValue = time() - mt_rand(1, 100000);

    $sql =
      'INSERT INTO '.
      $this->getTableNameForTests().
      ' (unit_test_stamp, int_data) VALUES( '.
      $ts.
      ', '.
      $expectedValue.
      ')';

    $dbh->setShardType($randShard);

    $insSth = $dbh->query($sql);

    $this->assertTrue($insSth->hasCursor());

    $this->assertEquals(1, $insSth->getNumRows());
    //, 'insSth: ' . var_export($insSth->fetchMap(), true));

    $sql =
      'SELECT unit_test_stamp, int_data FROM '.
      $this->getTableNameForTests().
      ' WHERE unit_test_stamp = '.
      $ts;

    $dbh->setShardType($randShard);

    $selSth = $dbh->query($sql);

    $this->assertEquals(
      1,
      $selSth->getNumRows(),
      'selSth: '.var_export($selSth->fetchMap(), true),
    );

  }

  public function testValidInsertQuery(): void {

    $dbh = $this->getDriverToTest();

    $randShard = $this->getRandomShardType();

    $ts = $this->getUnitTestStamp();

    $expectedValue = time();

    $sql =
      'INSERT INTO '.
      $this->getTableNameForTests().
      ' (unit_test_stamp, int_data) VALUES( '.
      $ts.
      ', '.
      $expectedValue.
      ')';

    $dbh->setShardType($randShard);

    $insSth = $dbh->query($sql);

    $this->assertTrue($insSth->hasCursor());

    $this->assertEquals(1, $insSth->getNumRows());

  }

  public function testValidInsertCommitQuery(): void {

    $dbh = $this->getDriverToTest();

    if ($dbh->getConfig()->isDatabaseReadOnly() === true) {
      $this->markTestSkipped(
        $dbh->getConfig()->getDatabaseName().'::isReadonly',
      );
      return;
    }

    $ts = $this->getUnitTestStamp();

    $expectedValue = time();

    // TODO: quote the values here with the quote infrastructure
    $sql =
      'INSERT INTO '.
      $this->getTableNameForTests().
      ' (unit_test_stamp, int_data) VALUES( '.
      $ts.
      ', '.
      $expectedValue.
      ')';

    $randShard = $this->getRandomShardType();

    $dbh->setShardType($randShard);

    $insSth = $dbh->query($sql);

    $this->assertTrue($insSth->wasSuccessful());

    $this->assertEquals(1, $insSth->getNumRows());

    $dbh->transaction()->commit();

  }

  public function testGetShardCount(): void {
    $dbh = $this->getDriverToTest();
    $this->assertTrue($dbh->getConfig()->getShardCount() > 0);
  }

}
