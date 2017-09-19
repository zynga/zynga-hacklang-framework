<?hh //strict

namespace Zynga\Framework\Database\V2\Driver;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Database\V2\Config\Mock\Cluster\Dev as DevCluster;
use Zynga\Framework\Database\V2\Driver\Mock;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;

class BaseTest extends TestCase {

  public function testConfig(): void {
    $obj = new Mock(new DevCluster());
    $this->assertEquals(DevCluster::class, get_class($obj->getConfig()));
  }

  public function testIsConnected(): void {
    $obj = new Mock(new DevCluster());
    $this->assertFalse($obj->getIsConnected());
  }

  public function testSetIsConnected(): void {
    $obj = new Mock(new DevCluster());
    $this->assertTrue($obj->setIsConnected(true));
  }

  public function testDriverStandup(): void {

    $config = new DevCluster();
    $driver = new Mock($config);

    $this->assertFalse($driver->isConnected());
    $this->assertTrue($driver->setIsConnected(false));

  }

  public function test_mockQueryToggles(): void {

    $config = new DevCluster();
    $driver = new Mock($config);

    $this->assertFalse($driver->getRequiresMockQueries());

    $this->assertTrue($driver->enableRequireMockQueries());
    $this->assertTrue($driver->getRequiresMockQueries());

    $this->assertTrue($driver->disableRequireMockQueries());
    $this->assertFalse($driver->getRequiresMockQueries());

  }

}
