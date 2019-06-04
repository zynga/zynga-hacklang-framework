<?hh // strict

namespace Zynga\Framework\Database\V2\Driver\Mock;

use Zynga\Framework\Database\V2\Config\Mock\Dev as DevConfig;
use
  Zynga\Framework\Database\V2\Driver\Mock\QueryableDriverThrowingQueryFailedException
;

use Zynga\Framework\Database\V2\Exceptions\QueryFailedException;
use Zynga\Framework\Testing\TestCase\V2\Base as TestBase;

class QueryableDriverThrowingQueryFailedExceptionTest extends TestBase {
  public function testRunQuery(): void {
    $config = new DevConfig();
    $obj = new QueryableDriverThrowingQueryFailedException($config);
    $this->expectException(QueryFailedException::class);
    $obj->query('some sql');
  }
}
