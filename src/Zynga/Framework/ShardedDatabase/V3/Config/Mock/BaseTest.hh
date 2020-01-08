<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Config\Mock;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\ShardedDatabase\V3\Factory as DatabaseFactory;
use Zynga\Framework\ShardedDatabase\V3\Config\Mock\Base as ConfigUnderTest;

use Zynga\Framework\ShardedDatabase\V3\Interfaces\DriverInterface;

abstract class BaseTest extends TestCase {

  public function testGetShardCount(): void {
    $config = $this->getConfigUnderTest();
    $this->assertTrue($config->getShardCount() > 0);
  }

  abstract public function getConfigUnderTest(): ConfigUnderTest;
}
