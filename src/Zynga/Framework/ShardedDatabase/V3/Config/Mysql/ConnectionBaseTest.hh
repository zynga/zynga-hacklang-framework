<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Mysql;

use Zynga\Framework\Database\V2\Exceptions\ConnectionGoneAwayException;
use Zynga\Framework\Database\V2\Exceptions\MissingUserIdException;
use Zynga\Framework\Database\V2\Exceptions\NoActiveCursorException;
use Zynga\Framework\Database\V2\Exceptions\OutOfBoundsForCursorException;
use Zynga\Framework\Database\V2\Exceptions\QueryException;
use Zynga\Framework\Database\V2\Exceptions\QueryFailedException;
use Zynga\Framework\Environment\DevelopmentMode\V1\DevelopmentMode;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use
  Zynga\Framework\ShardedDatabase\V3\Test\UserSharded\BadPDO\Mock as BadPDO_Mock
;

use Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO;
use
  Zynga\Framework\ShardedDatabase\V3\Driver\GenericPDO\ResultSet
;
use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use
  Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverConfigInterface
;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Poker\Type\Snid\V1\BoxFactory as SnidBoxFactory;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;

abstract class ConnectionBaseTest extends TestCase {
  private int $_unitTestStamp = 0;
  private string $_tableName = '';

  abstract public function getDriverName(): string;

  abstract public function getEnvironment(): int;

  abstract public function getSupportedSocialNetworks(): Vector<SnidBox>;

  abstract public function getTestUserIds(): Vector<UidBox>;

  abstract public function getSchemaForTests(): string;

  abstract public function getTableNameForTests(): string;

  public function getRandomSocialNetwork(): SnidBox {
    $vec = $this->getSupportedSocialNetworks();
    $vec->shuffle();
    return $vec[0];
  }

  public function getRandomTestUserId(): UidBox {
    $vec = $this->getTestUserIds();
    $vec->shuffle();
    return $vec[0];
  }

  public function doSetUpBeforeClass(): bool {

    // error_log('JEO doSetupParent');
    parent::doSetUpBeforeClass();

    $this->switchEnvironment();

    DatabaseFactory::clear();

    // error_log('JEO ENVMODE=' . DevelopmentMode::getModeAsString());

    return true;
  }

  public function switchEnvironment(): void {
    DevelopmentMode::reset();
    DevelopmentMode::setMode($this->getEnvironment());
  }

  public function resetEnvironment(): void {
    DevelopmentMode::reset();
  }

  public function getUnitTestStamp(): int {
    if ($this->_unitTestStamp === 0) {
      // we skew the time to allow for multiple people running unit tests
      // at the same time.
      $this->_unitTestStamp = time() - mt_rand(1, 10000000);
    }
    return $this->_unitTestStamp;
  }

  public function testConnection(): void {

    $sns = $this->getSupportedSocialNetworks();

    // Use an id of 1 to test if all the sns are okay [0 isn't allowed]
    $testUid = new UidBox(1);

    foreach ($sns as $sn) {

      $dbh = DatabaseFactory::factory(
        DriverInterface::class,
        $this->getDriverName(),
      );

      $dbh->setSnUid($sn, $testUid);

      // see if we have a connection string
      $this->assertNotEmpty(
        $dbh->getConfig()->getConnectionString($sn, $testUid),
      );

      // this is a misnomer as the query() call is in control off the connections.
      $this->assertTrue($dbh->connect());

    }

  }

  public function testValidQuery(): void {

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );

    $sns = $this->getSupportedSocialNetworks();
    $uids = $this->getTestUserIds();

    foreach ($sns as $sn) {

      foreach ($uids as $uid) {

        $expectedValue = time();

        $dbh->setSnUid($sn, $uid);

        $sth = $dbh->query('SELECT '.$expectedValue.' as \'val\' FROM dual');

        $this->assertNotFalse($sth->wasSuccessful());
        $this->assertInstanceOf(ResultSet::class, $sth);

        // this will fetch the row count un-cached
        $cnt = $sth->getNumRows();
        $this->assertEquals(1, $cnt);

        // this should be cached
        $cnt = $sth->getNumRows();
        $this->assertEquals(1, $cnt);

      }

    }

  }

  public function testValidQueryFetchMap(): void {

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );

    $expectedValue = time();
    $sns = $this->getSupportedSocialNetworks();
    $uids = $this->getTestUserIds();

    foreach ($sns as $sn) {

      foreach ($uids as $uid) {

        $dbh->setSnUid($sn, $uid);

        $sth = $dbh->query('SELECT '.$expectedValue.' as \'val\' FROM dual');

        $this->assertTrue($sth->wasSuccessful());

        $this->assertTrue($sth->next());

        $map = $sth->fetchMap();

        $this->assertInstanceOf(Map::class, $map);

        if ($map instanceof Map) {
          $this->assertEquals($expectedValue, $map->get('val'));
          $this->assertEquals($expectedValue, $map['val']);
        }

        // there should only be one row here, nothing more, nothing less.
        $this->assertEquals(1, $sth->getNumRows());

        $this->assertFalse($sth->hasMore());

        // this should fail to index
        $this->assertFalse($sth->next());
      }
    }

  }

  public function testValidQueryFetchVector(): void {

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );

    $expectedValue = time();

    $sns = $this->getSupportedSocialNetworks();
    $uids = $this->getTestUserIds();

    foreach ($sns as $sn) {
      foreach ($uids as $uid) {

        $dbh->setSnUid($sn, $uid);

        $sth = $dbh->query('SELECT '.$expectedValue.' as \'val\' FROM dual');

        $this->assertTrue($sth->wasSuccessful());

        $this->assertTrue($sth->hasMore());

        $this->assertTrue($sth->next());

        $vector = $sth->fetchVector();

        $this->assertInstanceOf(Vector::class, $vector);

        if ($vector instanceof Vector) {
          $this->assertEquals($expectedValue, $vector[0]);
        }

        // there should only be one row here, nothing more, nothing less.
        $this->assertEquals(1, $sth->getNumRows());

        $this->assertFalse($sth->hasMore());

        // this should fail to index
        $this->assertFalse($sth->next());

      }
    }

  }

  public function testValidQueryFetchMapCursorGone(): void {

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $expectedValue = time();

    $sns = $this->getSupportedSocialNetworks();
    $uids = $this->getTestUserIds();

    $sns->shuffle();
    $uids->shuffle();

    $dbh->setSnUid($sns[0], $uids[0]);

    $sth = $dbh->query('SELECT '.$expectedValue.' as \'val\' FROM dual');

    $sth->freeCursor();

    $this->expectException(NoActiveCursorException::class);
    $map = $sth->fetchMap();

  }

  public function testValidQueryFetchVectorCursorGone(): void {

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $expectedValue = time();

    $sns = $this->getSupportedSocialNetworks();
    $uids = $this->getTestUserIds();

    $sns->shuffle();
    $uids->shuffle();

    $dbh->setSnUid($sns[0], $uids[0]);

    $sth = $dbh->query('SELECT '.$expectedValue.' as \'val\' FROM dual');

    $sth->freeCursor();

    $this->expectException(NoActiveCursorException::class);
    $vector = $sth->fetchVector();

  }

  public function testValidQueryRewind(): void {

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );

    $expectedValues = array();

    $expectedValues[] = time().'_1';
    $expectedValues[] = time().'_2';
    $expectedValues[] = time().'_3';

    $expectedValuesCount = count($expectedValues);

    $sql =
      "SELECT '".
      $expectedValues[0].
      "' as 'val' FROM dual ".
      "UNION ALL SELECT '".
      $expectedValues[1].
      "' as 'val' FROM dual ".
      "UNION ALL SELECT '".
      $expectedValues[2].
      "' as 'val' FROM dual ";

    $dbh->setSnUid(
      $this->getRandomSocialNetwork(),
      $this->getRandomTestUserId(),
    );

    $sth = $dbh->query($sql);

    for ($i = 0; $i < $expectedValuesCount; $i++) {

      // fetch a map really quick
      $sth->next();

      $map = $sth->fetchMap();

      $this->assertInstanceOf(Map::class, $map);

      $this->assertEquals($expectedValues[$i], $map['val']);
    }

    // now rewind
    $this->assertTrue($sth->rewind(0));

    for ($i = 0; $i < $expectedValuesCount; $i++) {

      // fetch a map really quick
      $sth->next();

      $map = $sth->fetchMap();

      $this->assertInstanceOf(Map::class, $map);

      $this->assertEquals($expectedValues[$i], $map['val']);
    }

    // now rewind
    $this->assertTrue($sth->rewind(0));

    for ($i = 0; $i < 20; $i++) {

      $offset = mt_rand(0, $expectedValuesCount);

      // now rewind
      $this->assertTrue($sth->rewind($offset));

    }

  }

  public function testValidQueryWasSqlDML(): void {

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $expectedValue = time();

    $dbh->setSnUid(
      $this->getRandomSocialNetwork(),
      $this->getRandomTestUserId(),
    );

    $sth = $dbh->query('SELECT '.$expectedValue.' as \'val\' FROM dual');

    // test that we can detect dml in it's different forms
    $dmls = array();
    $dmls[] = 'DELETE FROM jeo.doesnotexist';
    $dmls[] = 'insert INTO jeo.doesnotexist (name) VALUES ("something")';
    $dmls[] = 'update jeo.doesnotexist set name = "something" where id = 5';

    // snag the current sql and save it.
    $currentSql = $sth->getSql();

    foreach ($dmls as $dml) {
      $sth->setSql($dml);
      $this->assertEquals($dml, $sth->getSql());
      $this->assertTrue($sth->wasSqlDML());
    }

    // reset the current sql and see if it was dml to eercise the false case again.
    $sth->setSql($currentSql);
    $this->assertFalse($sth->wasSqlDML());

  }

  public function testValidQueryHasCursor(): void {
    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $expectedValue = time();

    $dbh->setSnUid(
      $this->getRandomSocialNetwork(),
      $this->getRandomTestUserId(),
    );

    $sth = $dbh->query('SELECT '.$expectedValue.' as \'val\' FROM dual');

    $this->assertTrue($sth->hasCursor());
  }

  public function testValidHadError(): void {
    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $this->assertFalse($dbh->hadError());
    $this->assertEquals('', $dbh->getLastError());
  }

  public function testInvalidateCursor(): void {
    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $expectedValue = time();

    $dbh->setSnUid(
      $this->getRandomSocialNetwork(),
      $this->getRandomTestUserId(),
    );

    $sth = $dbh->query("SELECT $expectedValue as 'val' FROM dual");
    $this->assertTrue($sth->hasCursor());
    $this->assertTrue($sth->freeCursor());
    $this->assertFalse($sth->hasCursor());
    $this->assertFalse($sth->freeCursor());
    $this->assertFalse($sth->wasSuccessful());
  }

  public function testInvaildQuery(): void {

    $expectedValue = time();
    $schema = $this->getSchemaForTests();

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );

    $dbh->setSnUid(
      $this->getRandomSocialNetwork(),
      $this->getRandomTestUserId(),
    );

    $this->expectException(QueryFailedException::class);

    $sth = $dbh->query("SELECT $expectedValue as 'val'' FROM dual");

  }

  public function testHadError(): void {

    $expectedValue = time();
    $schema = $this->getSchemaForTests();

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );

    try {

      $dbh->setSnUid(
        $this->getRandomSocialNetwork(),
        $this->getRandomTestUserId(),
      );

      $sth = $dbh->query(
        "SELECT $expectedValue as 'val' FROM $schema.table_does_not_exist",
      );

    } catch (Exception $e) {
      // do nothing, as we shouldn't be getting to here.
    }

    $this->assertTrue($dbh->hadError());

    $this->assertEquals(
      "SQLSTATE[42S02] [1146] Table '$schema.table_does_not_exist' doesn't exist",
      $dbh->getLastError(),
    );

  }

  public function testInvaildQueryErrorMessage(): void {
    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $expectedValue = time();

    $dbh->setSnUid(
      $this->getRandomSocialNetwork(),
      $this->getRandomTestUserId(),
    );

    $this->expectException(QueryFailedException::class);
    $sth = $dbh->query("SELECT $expectedValue as 'val'' FROM dual");

    $this->assertTrue($dbh->hadError());

    $this->assertEquals(
      'ERROR:  Relation "schemadoesnotexist.table_does_not_exist" does not exist',
      $dbh->getLastError(),
    );

  }

  abstract public function testValidInsertCommitSelectQuery(): void;
  abstract public function testValidInsertQuery(): void;
  abstract public function testValidInsertCommitQuery(): void;

  public function testNumRowsException(): void {

    $expectedValue = time();

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );

    $dbh->setSnUid(
      $this->getRandomSocialNetwork(),
      $this->getRandomTestUserId(),
    );

    $sth = $dbh->query("SELECT $expectedValue as 'val' FROM dual");

    $this->assertTrue($sth->hasCursor());
    $this->assertEquals(1, $sth->getNumRows());
    $this->assertTrue($sth->freeCursor());

    // finally emit the exception
    $this->expectException(NoActiveCursorException::class);
    $sth->getNumRows();
  }

  public function testNextException(): void {

    $expectedValue = time();

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );

    $dbh->setSnUid(
      $this->getRandomSocialNetwork(),
      $this->getRandomTestUserId(),
    );

    $sth = $dbh->query("SELECT $expectedValue as 'val' FROM dual");

    $this->assertTrue($sth->hasCursor());
    $this->assertEquals(1, $sth->getNumRows());
    $this->assertTrue($sth->freeCursor());
    // finally emit the exception
    $this->expectException(NoActiveCursorException::class);
    $sth->next();
  }

  public function testHasMoreException(): void {

    $expectedValue = time();

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );

    $dbh->setSnUid(
      $this->getRandomSocialNetwork(),
      $this->getRandomTestUserId(),
    );

    $sth = $dbh->query("SELECT $expectedValue as 'val' FROM dual");

    $this->assertTrue($sth->hasCursor());
    $this->assertEquals(1, $sth->getNumRows());
    $this->assertTrue($sth->freeCursor());
    // finally emit the exception
    $this->expectException(NoActiveCursorException::class);
    $sth->hasMore();
  }

  public function testInvalidQueryRewindPos(): void {

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $expectedValue = time();

    $dbh->setSnUid(
      $this->getRandomSocialNetwork(),
      $this->getRandomTestUserId(),
    );

    $sth = $dbh->query("SELECT $expectedValue as 'val' FROM dual");

    $this->expectException(OutOfBoundsForCursorException::class);
    $sth->rewind(1234);

  }

  public function testInvalidQueryRewindNeg(): void {

    $expectedValue = time();

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );

    $dbh->setSnUid(
      $this->getRandomSocialNetwork(),
      $this->getRandomTestUserId(),
    );

    $sth = $dbh->query("SELECT $expectedValue as 'val' FROM dual");

    $this->expectException(OutOfBoundsForCursorException::class);
    $sth->rewind(-1234);

  }

  public function testQueryRewindNoCursor(): void {

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $expectedValue = time();

    $dbh->setSnUid(
      $this->getRandomSocialNetwork(),
      $this->getRandomTestUserId(),
    );

    $sth = $dbh->query("SELECT $expectedValue as 'val' FROM dual");

    $sth->freeCursor();

    $this->expectException(NoActiveCursorException::class);
    $sth->rewind(0);

  }

  public function testQueryConnectionGone(): void {

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $dbh->setSnUid(SnidBoxFactory::facebook(), new UidBox(1));
    $dbh->disconnect();
    $expectedValue = time();

    $sns = $this->getSupportedSocialNetworks();
    $uids = $this->getTestUserIds();

    $sns->shuffle();
    $uids->shuffle();

    $dbh->setSnUid($sns[0], $uids[0]);

    //update with wrong password
    $server = $dbh->getConfig()->getServerFromUserId($sns[0], $uids[0]);
    $server->setPassword('the-wrong-password');

    $this->expectException(ConnectionGoneAwayException::class);

    $sth = $dbh->query("SELECT $expectedValue as 'val' FROM dual");

  }

  public function testFailToConnect(): void {

    // remove the valid driver (if it exsists)
    $this->cleanDriverFromFactory();

    $sns = $this->getSupportedSocialNetworks();
    $uids = $this->getTestUserIds();

    $sns->shuffle();
    $uids->shuffle();

    // squidge the database connection by changing the password to invalid one.
    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $dbh->setSnUid(SnidBoxFactory::facebook(), new UidBox(1));

    //update with wrong password
    $server =
      $dbh->getConfig()
        ->getServerFromUserId(SnidBoxFactory::facebook(), new UidBox(1));
    $server->setPassword('some-made-up-estimate');

    $this->expectException(ConnectionGoneAwayException::class);
    $this->assertFalse($dbh->connect());

    // cleanup..
    $this->cleanDriverFromFactory();
  }

  public function testConnectRequiredBadSocialNetwork(): void {
    // remove the valid driver (if it exsists)
    $this->cleanDriverFromFactory();
    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $this->expectException(MissingUserIdException::class);
    $dbh->connect();
  }

  public function testConnectRequiredUserid(): void {
    // remove the valid driver (if it exsists)
    $this->cleanDriverFromFactory();
    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $this->expectException(MissingUserIdException::class);
    $dbh->connect();
  }

  public function testConnectRequiredBadUserid(): void {
    // remove the valid driver (if it exsists)
    $this->cleanDriverFromFactory();
    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $this->expectException(MissingUserIdException::class);
    $dbh->connect();
  }

  public function testQuoteInterface(): void {

    $this->cleanDriverFromFactory();

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $testValue = "don't";
    $expectedValue = "'don\\'t'";

    $randoSocialNetwork = $this->getRandomSocialNetwork();
    $randoTestUserId = $this->getRandomTestUserId();

    $dbh->setSnUid($randoSocialNetwork, $randoTestUserId);

    $this->assertEquals($expectedValue, $dbh->quote()->textValue($testValue));

  }

  public function cleanDriverFromFactory(): void {
    DatabaseFactory::clear();
  }

  public function testBadConnectionQuoteInterface(): void {

    $this->cleanDriverFromFactory();

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );

    // 1,1
    $testValue = "don't";
    $expectedValue = "'don\\'t'";
    $randoSocialNetwork = $this->getRandomSocialNetwork();
    $randoTestUserId = $this->getRandomTestUserId();
    $dbh->setSnUid($randoSocialNetwork, $randoTestUserId);

    //update with wrong password
    $server =
      $dbh->getConfig()
        ->getServerFromUserId($randoSocialNetwork, $randoTestUserId);
    $server->setPassword('something-that-we-wont-use');

    $this->expectException(ConnectionGoneAwayException::class);

    $this->assertEquals($expectedValue, $dbh->quote()->textValue($testValue));

  }

  public function testFloatValues(): void {

    $driver = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );

    $quoter = $driver->getQuoter();

    $floatValues = Map {};
    $floatValues['0.0'] = 0.0;
    $floatValues['1.0'] = 1.00;
    $floatValues['0.01'] = 0.01;
    $floatValues['0.1789'] = 0.1789;

    $testSn = SnidBoxFactory::facebook();
    $testUid = new UidBox(1);

    $driver->setSnUid($testSn, $testUid);

    foreach ($floatValues as $floatExpected => $floatValue) {
      $this->assertEquals($floatExpected, $quoter->floatValue($floatValue));
    }

    $this->cleanDriverFromFactory();

  }

  public function testIntValues(): void {

    $driver = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );

    $quoter = $driver->getQuoter();

    $intValues = Map {};
    $intValues['0'] = 0;
    $intValues['1'] = 1;
    $intValues['10000'] = 10000;

    $rando = mt_rand(1, mt_getrandmax());
    $intValues["$rando"] = $rando;

    $testSn = SnidBoxFactory::facebook();
    $testUid = new UidBox(1);

    $driver->setSnUid($testSn, $testUid);

    foreach ($intValues as $intExpected => $intValue) {
      $this->assertEquals($intExpected, $quoter->intValue($intValue));
    }

    $this->cleanDriverFromFactory();

  }

  public function testTextValues(): void {

    $driver = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );

    $quoter = $driver->getQuoter();

    $testSn = SnidBoxFactory::facebook();
    $testUid = new UidBox(1);

    $driver->setSnUid($testSn, $testUid);

    $this->assertEquals(
      "'i am a teapot'",
      $quoter->textValue('i am a teapot'),
    );

    $vec = Vector {};
    $vec[] = 'you have to catch them all';
    $vec[] = 'as you are the chosen one';

    $this->assertEquals(
      "'you have to catch them all', 'as you are the chosen one'",
      $quoter->textVector($vec, $testSn, $testUid),
    );

    $this->cleanDriverFromFactory();

  }

  public function testTransactionObject(): void {

    $dbh = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );

    $randSN = $this->getRandomSocialNetwork();
    $randUID = $this->getRandomTestUserId();

    $dbh->setSnUid($randSN, $randUID);

    $this->assertTrue($dbh->transaction()->begin());
    $this->assertTrue($dbh->transaction()->commit());
    $this->assertTrue($dbh->transaction()->rollback());

    $this->cleanDriverFromFactory();

  }

  public function testBadNativeQuoteString(): void {
    $driver = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $mock = new BadPDO_Mock($driver->getConfig());
    $this->expectException(Exception::class);
    $mock->nativeQuoteString('a-string');
  }

  public function testBadBegin(): void {
    $driver = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $mock = new BadPDO_Mock($driver->getConfig());
    $this->expectException(Exception::class);
    $mock->getTransaction()->begin();
  }

  public function testBadCommit(): void {
    $driver = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $mock = new BadPDO_Mock($driver->getConfig());
    $this->expectException(Exception::class);
    $mock->getTransaction()->commit();
  }

  public function testBadRollback(): void {
    $driver = DatabaseFactory::factory(
      DriverInterface::class,
      $this->getDriverName(),
    );
    $mock = new BadPDO_Mock($driver->getConfig());
    $this->expectException(Exception::class);
    $mock->getTransaction()->rollback();
  }

}
