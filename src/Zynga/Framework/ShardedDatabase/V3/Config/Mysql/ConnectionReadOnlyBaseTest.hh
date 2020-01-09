<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Mysql;

use Zynga\Framework\Database\V2\Exceptions\ConnectionIsReadOnly;
use Zynga\Framework\Factory\V2\Test\MockState as FactoryMockState;

use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\ShardedDatabase\V3\Config\Mysql\ConnectionBaseTest;
use Zynga\Framework\Type\V1\UInt64Box;

abstract class ConnectionReadOnlyBaseTest
  extends ConnectionBaseTest<UInt64Box> {

  public function doSetUpBeforeClass(): bool {

    parent::doSetUpBeforeClass();

    DatabaseFactory::disableMockDrivers();

    return true;

  }

  public function testValidInsertCommitSelectQuery(): void {
    // NOOP: As you can't do a insert nor commit on a read only connection.
    $this->assertFalse(false);
  }

  public function testValidInsertQuery(): void {

    $dbh = $this->getDriverToTest();
    $ts = $this->getUnitTestStamp();

    $expectedValue = time();

    $sql =
      'INSERT INTO dual (unit_test_stamp, int_data) VALUES( '.
      $ts.
      ', '.
      $expectedValue.
      ')';

    //$dbh->setSnUid($randSN, $randUID);

    $this->expectException(ConnectionIsReadOnly::class);
    $insSth = $dbh->query($sql);

  }

  public function testValidInsertCommitQuery(): void {
    // NOOP: As you can't do a insert nor commit on a read only connection.
    $this->assertFalse(false);
  }

  public function testReadOnlyException(): void {

    $this->cleanDriverFromFactory();

    $dbh = $this->getDriverToTest();

    if ($dbh->getConfig()->isDatabaseReadOnly() === false) {
      $this->markTestSkipped(
        $dbh->getConfig()->getDatabaseName().'::isReadWrite',
      );
      return;
    }

    $this->expectException(ConnectionIsReadOnly::class);
    $dbh->query('INSERT INTO dual VALUES (1,1);');

  }

  public function testGetShardCount(): void {
    $dbh = $this->getDriverToTest();
    $this->assertTrue($dbh->getConfig()->getShardCount() > 0);
  }

}
