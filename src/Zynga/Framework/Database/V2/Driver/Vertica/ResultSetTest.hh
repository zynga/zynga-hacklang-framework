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

  public static function goodDriverForTestProvider(): DriverInterface {
    DatabaseFactory::clear();
    // JEO: We need a testing configuration for vertica.
    return DatabaseFactory::factory(DriverInterface::class, 'Test_Vertica');
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testWasSuccessfulFalse(DriverInterface $dbh): void {
    $sth = $dbh->query('SELECT 1');
    $sth->freeCursor();
    $this->assertFalse($sth->wasSuccessful());
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testWasSuccessfulTrue(DriverInterface $dbh): void {
    $sth = $dbh->query('SELECT 1 WHERE 1 = 2');
    $this->assertTrue($sth->wasSuccessful());
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testFreeCursorTrue(DriverInterface $dbh): void {
    $sth = $dbh->query('SELECT 1 WHERE 1 = 2');
    $this->assertTrue($sth->wasSuccessful());
    $this->assertTrue($sth->freeCursor());
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testFreeCursorFalse(DriverInterface $dbh): void {
    $sth = $dbh->query('SELECT 1 WHERE 1 = 2');
    $this->assertTrue($sth->wasSuccessful());
    $this->assertTrue($sth->freeCursor());
    $this->assertFalse($sth->freeCursor());
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testGetNumRowsZero(DriverInterface $dbh): void {
    $sth = $dbh->query('SELECT 1 WHERE 1 = 2');
    $this->assertEquals(0, $sth->getNumRows());
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testGetNumRowsNoActiveCursorException(
    DriverInterface $dbh,
  ): void {
    $sth = $dbh->query('SELECT 1 WHERE 1 = 2');
    $sth->freeCursor();
    $this->expectException(NoActiveCursorException::class);
    $sth->getNumRows();
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testGetNumRowsZeroTwice(DriverInterface $dbh): void {
    $sth = $dbh->query('SELECT 1 WHERE 1 = 2');
    $this->assertEquals(0, $sth->getNumRows());
    $this->assertEquals(0, $sth->getNumRows());
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testRewindNoActiveCursorException(
    DriverInterface $dbh,
  ): void {
    $sth = $dbh->query('SELECT 1 WHERE 1 = 2');
    $sth->freeCursor();
    $this->expectException(NoActiveCursorException::class);
    $sth->rewind(0);
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testRewindNegativePosition(DriverInterface $dbh): void {
    $sth = $dbh->query('SELECT 1');
    $this->expectException(OutOfBoundsForCursorException::class);
    $sth->rewind(-1);
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testValidRewind(DriverInterface $dbh): void {
    $sth = $dbh->query('SELECT 1');
    $this->assertTrue($sth->rewind(0));
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testHasMoreFalse(DriverInterface $dbh): void {
    $sth = $dbh->query('SELECT 1 WHERE 1 = 2');
    $this->assertFalse($sth->hasMore());
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testNextNoActiveCursorException(DriverInterface $dbh): void {
    $sth = $dbh->query('SELECT 1 WHERE 1 = 2');
    $sth->freeCursor();
    $this->expectException(NoActiveCursorException::class);
    $sth->next();
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testNextNoneLeft(DriverInterface $dbh): void {
    $sth = $dbh->query('SELECT 1');
    $this->assertTrue($sth->next());
    $this->assertFalse($sth->next());
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testFetchMapNoActiveCursorException(
    DriverInterface $dbh,
  ): void {
    $sth = $dbh->query('SELECT 1');
    $sth->freeCursor();
    $this->expectException(NoActiveCursorException::class);
    $sth->fetchMap();
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testFetchMapOneResult(DriverInterface $dbh): void {
    $sth = $dbh->query('SELECT 1 as "monkey"');
    $sth->next();
    $this->assertEquals(1, count($sth->fetchMap()));
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testFetchVectorNoActiveCursorException(
    DriverInterface $dbh,
  ): void {
    $sth = $dbh->query("SELECT 1");
    $sth->freeCursor();
    $this->expectException(NoActiveCursorException::class);
    $sth->fetchVector();
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testFetchVectorOneResult(DriverInterface $dbh): void {
    $sth = $dbh->query("SELECT 1");
    $sth->next();
    $this->assertEquals(1, count($sth->fetchVector()));
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testFetchVectorTwoResult(DriverInterface $dbh): void {
    $sth = $dbh->query("SELECT 1, 2");
    $sth->next();
    $this->assertEquals(2, count($sth->fetchVector()));
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testSetSqlSuccess(DriverInterface $dbh): void {
    $sth = $dbh->query("SELECT 1");
    $this->assertTrue($sth->setSql(''));
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testGetSqlSuccess(DriverInterface $dbh): void {
    $sql = 'SELECT 1';
    $sth = $dbh->query($sql);
    $this->assertEquals($sql, $sth->getSql());
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testWasSqlDMLFalse(DriverInterface $dbh): void {
    $sql = 'SELECT 1';
    $sth = $dbh->query($sql);
    $this->assertFalse($sth->wasSqlDML());
  }

  <<dataProvider("goodDriverForTestProvider")>>
  public function testWasSqlDMLTrue(DriverInterface $dbh): void {
    $sql = 'INSERT INTO dual (baz) VALUES (123)';
    $sth = $dbh->query('SELECT 1');
    // yea you can do this currently.
    $sth->setSql($sql);
    $this->assertTrue($sth->wasSqlDML());
  }

}
