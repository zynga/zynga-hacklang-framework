<?hh //strict

namespace Zynga\Framework\Database\V2;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Environment\DevelopmentMode\V1\DevelopmentMode;

use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Driver as DatabaseDriver;

use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\DriverConfigInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;

use Zynga\Framework\Database\V2\Driver\Dummy as NativeDummyDriver;
use Zynga\Framework\Database\V2\Driver\Dummy\Quoter as NativeDummyQuoter;
use Zynga\Framework\Database\V2\Driver\Dummy\Transaction as NativeDummyTransaction;

use Zynga\Framework\Database\V2\Config\Base as ConfigBase;

use Zynga\Framework\Database\V2\Exceptions\FailedToLoadDriverException;
use Zynga\Framework\Database\V2\Exceptions\FailedToLoadDriverConfigException;
use Zynga\Framework\Database\V2\Exceptions\Mock\LoadTestDataNoFileFoundException;

class FactoryTest extends TestCase {

  public function test_loadResultsForTest_FileNotExist(): void {
    $this->expectException(LoadTestDataNoFileFoundException::class);
    DatabaseFactory::loadResultsForTest(DatabaseFactory::class, __FUNCTION__);
  }
}
