<?hh //strict

namespace Zynga\Framework\Database\V2\Driver\Vertica;

use Zynga\Framework\Database\V2\Exceptions\NoActiveCursorException;
use Zynga\Framework\Database\V2\Exceptions\OutOfBoundsForCursorException;

use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class ResultSetTest extends TestCase {

  public function doSetUpBeforeClass(): bool {

    parent::doSetUpBeforeClass();

    DatabaseFactory::disableMockDrivers();

    return true;

  }

  public function getDriverForTest(): DriverInterface {
    DatabaseFactory::clear();
    // JEO: We need a testing configuration for vertica.
    return DatabaseFactory::factory(DriverInterface::class, 'PHT_Loader');
  }

  public function testWasSuccessfulFalse(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query('SELECT 1 FROM dual');
    $sth->freeCursor();
    $this->assertFalse($sth->wasSuccessful());
  }

  public function testWasSuccessfulTrue(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query('SELECT 1 FROM dual WHERE 1 = 2');
    $this->assertTrue($sth->wasSuccessful());
  }

  public function testFreeCursorTrue(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query('SELECT 1 FROM dual WHERE 1 = 2');
    $this->assertTrue($sth->wasSuccessful());
    $this->assertTrue($sth->freeCursor());
  }

  public function testFreeCursorFalse(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query('SELECT 1 FROM dual WHERE 1 = 2');
    $this->assertTrue($sth->wasSuccessful());
    $this->assertTrue($sth->freeCursor());
    $this->assertFalse($sth->freeCursor());
  }

  public function testGetNumRowsZero(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query('SELECT 1 FROM dual WHERE 1 = 2');
    $this->assertEquals(0, $sth->getNumRows());
  }

  public function testGetNumRowsNoActiveCursorException(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query('SELECT 1 FROM dual WHERE 1 = 2');
    $sth->freeCursor();
    $this->expectException(NoActiveCursorException::class);
    $sth->getNumRows();
  }

  public function testGetNumRowsZeroTwice(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query('SELECT 1 FROM dual WHERE 1 = 2');
    $this->assertEquals(0, $sth->getNumRows());
    $this->assertEquals(0, $sth->getNumRows());
  }

  public function testRewindNoActiveCursorException(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query('SELECT 1 FROM dual WHERE 1 = 2');
    $sth->freeCursor();
    $this->expectException(NoActiveCursorException::class);
    $sth->rewind(0);
  }

  public function testRewindNegativePosition(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query('SELECT 1 FROM dual');
    $this->expectException(OutOfBoundsForCursorException::class);
    $sth->rewind(-1);
  }

  public function testValidRewind(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query('SELECT 1 FROM dual');
    $this->assertTrue($sth->rewind(0));
  }

  public function testHasMoreFalse(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query('SELECT 1 FROM dual WHERE 1 = 2');
    $this->assertFalse($sth->hasMore());
  }

  public function testNextNoActiveCursorException(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query('SELECT 1 FROM dual WHERE 1 = 2');
    $sth->freeCursor();
    $this->expectException(NoActiveCursorException::class);
    $sth->next();
  }

  public function testNextNoneLeft(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query('SELECT 1 FROM dual');
    $this->assertTrue($sth->next());
    $this->assertFalse($sth->next());
  }


  public function testFetchMapNoActiveCursorException(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query('SELECT 1 FROM dual');
    $sth->freeCursor();
    $this->expectException(NoActiveCursorException::class);
    $sth->fetchMap();
  }

  public function testFetchMapOneResult(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query("SELECT 1 as 'monkey' FROM dual");
    $sth->next();
    $this->assertEquals(1, count($sth->fetchMap()));
  }

  public function testFetchVectorNoActiveCursorException(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query("SELECT 1 FROM dual");
    $sth->freeCursor();
    $this->expectException(NoActiveCursorException::class);
    $sth->fetchVector();
  }

  public function testFetchVectorOneResult(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query("SELECT 1 FROM dual");
    $sth->next();
    $this->assertEquals(1, count($sth->fetchVector()));
  }

  public function testFetchVectorTwoResult(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query("SELECT 1, 2 FROM dual");
    $sth->next();
    $this->assertEquals(2, count($sth->fetchVector()));
  }

  public function testSetSqlSuccess(): void {
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query("SELECT 1 FROM dual");
    $this->assertTrue($sth->setSql(''));
  }

  public function testGetSqlSuccess(): void {
    $sql = 'SELECT 1 FROM dual';
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query($sql);
    $this->assertEquals($sql, $sth->getSql());
  }

  public function testWasSqlDMLFalse(): void {
    $sql = 'SELECT 1 FROM dual';
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query($sql);
    $this->assertFalse($sth->wasSqlDML());
  }

  public function testWasSqlDMLTrue(): void {
    $sql = 'INSERT INTO dual (baz) VALUES (123)';
    $dbh = $this->getDriverForTest();
    $sth = $dbh->query('SELECT 1 FROM dual');
    // yea you can do this currently.
    $sth->setSql($sql);
    $this->assertTrue($sth->wasSqlDML());
  }

}
