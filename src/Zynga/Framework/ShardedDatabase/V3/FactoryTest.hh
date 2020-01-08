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

class FactoryTest extends TestCase {

  public function test_loadResultsForTest_FileNotExist(): void {
    $this->expectException(LoadTestDataNoFileFoundException::class);
    DatabaseFactory::loadResultsForTest(DatabaseFactory::class, __FUNCTION__);
  }
  
  public function testGetRead(): void {
    $dbh =
      DatabaseFactory::getMockDriver();
    $this->assertTrue($dbh instanceof DriverInterface);
  }
}
