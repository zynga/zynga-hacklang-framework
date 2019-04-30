<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3;

use
  Zynga\Framework\Database\V2\Exceptions\Mock\LoadTestDataNoFileFoundException
;
use Zynga\Framework\Database\V2\Exceptions\MissingUserIdException;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use
  Zynga\Framework\Type\V1\Exceptions\FailedToImportInvalidSocialNetworkException
;

use
  Zynga\Framework\ShardedDatabase\V3\Driver\Iterator\Base as DriverIterator
;
use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;
use Zynga\Poker\Type\Snid\V1\BoxFactory as SnidBoxFactory;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;

class FactoryTest extends TestCase {

  public function test_loadResultsForTest_FileNotExist(): void {
    $this->expectException(LoadTestDataNoFileFoundException::class);
    DatabaseFactory::loadResultsForTest(DatabaseFactory::class, __FUNCTION__);
  }

  public function testGetRead(): void {
    $dbh =
      DatabaseFactory::getRead(SnidBoxFactory::facebook(), new UidBox(1));
    $this->assertTrue($dbh instanceof DriverInterface);
  }

  public function test_GetRead_Sn_Failure(): void {
    $this->expectException(
      FailedToImportInvalidSocialNetworkException::class,
    );
    $dbh = DatabaseFactory::getRead(new SnidBox(0), new UidBox(1));
    // $dbh->query('SELECT * FROM dual');
  }

  public function testGetRead_Uid_Failure(): void {
    $this->expectException(MissingUserIdException::class);
    $dbh =
      DatabaseFactory::getRead(SnidBoxFactory::facebook(), new UidBox(0));
    // $dbh->query('SELECT * FROM dual');
  }

  public function testGetWrite(): void {
    $dbh =
      DatabaseFactory::getWrite(SnidBoxFactory::facebook(), new UidBox(1));
    $this->assertTrue($dbh instanceof DriverInterface);
  }

  public function testGetWrite_Sn_Failure(): void {
    $this->expectException(
      FailedToImportInvalidSocialNetworkException::class,
    );
    $dbh = DatabaseFactory::getWrite(new SnidBox(0), new UidBox(1));
    // $dbh->query('SELECT * FROM dual');
  }

  public function testGetWrite_Uid_Failure(): void {
    $this->expectException(MissingUserIdException::class);
    $dbh =
      DatabaseFactory::getWrite(SnidBoxFactory::facebook(), new UidBox(0));
    // $dbh->query('SELECT * FROM dual');
  }

  public function testGetReadIterator(): void {
    $readIterator =
      DatabaseFactory::getReadIterator(SnidBoxFactory::facebook());
    $this->assertTrue($readIterator instanceof DriverIterator);
  }

  public function testGetSexyRead(): void {
    $dbh =
      DatabaseFactory::getSexyRead(SnidBoxFactory::facebook(), new UidBox(1));
    $this->assertTrue($dbh instanceof DriverInterface);
  }

  public function testGetSexyWrite(): void {
    $dbh = DatabaseFactory::getSexyWrite(
      SnidBoxFactory::facebook(),
      new UidBox(1),
    );
    $this->assertTrue($dbh instanceof DriverInterface);
  }

  public function testGetSexyGDPRRead(): void {
    $dbh = DatabaseFactory::getSexyGDPRRead(
      SnidBoxFactory::facebook(),
      new UidBox(1),
    );
    $this->assertTrue($dbh instanceof DriverInterface);
  }

  public function testGetSexyGDPRWrite(): void {
    $dbh = DatabaseFactory::getSexyGDPRWrite(
      SnidBoxFactory::facebook(),
      new UidBox(1),
    );
    $this->assertTrue($dbh instanceof DriverInterface);
  }

}
