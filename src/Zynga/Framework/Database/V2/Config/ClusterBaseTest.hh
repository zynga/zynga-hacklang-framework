<?hh //strict

namespace Zynga\Framework\Database\V2\Config;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Database\V2\Config\ClusterBase;
use Zynga\Framework\Database\V2\DriverConfigInterface;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;

use Zynga\Framework\Database\V2\Driver\Mock as MockDriver;
use Zynga\Framework\Database\V2\Driver\Mock\Quoter as MockQuoter;
use Zynga\Framework\Database\V2\Driver\Mock\Transaction as MockTransaction;
use
  Zynga\Framework\Database\V2\Config\Mock\Cluster\Broken\Dev as BrokenClusterInit
;
use Zynga\Framework\Database\V2\Config\Mock\Cluster\Dev as MockCluster;
use Zynga\Framework\Database\V2\Exceptions\ClusterInitFailureException;
use Zynga\Framework\Database\V2\Exceptions\ClusterInitNoServersException;
use
  Zynga\Framework\Database\V2\Config\Mock\Cluster\NoServersAdded\Dev as BrokenClusterNoServers
;

class ClusterBaseTest extends TestCase {

  /**
   * Test the username
   */
  public function testUserName(): void {

    $config = new MockCluster();

    $expected = 'pokertestvalue';

    $rv = $config->setUserName($expected);

    $this->assertTrue($rv);

    $value = $config->getUserName();

    $this->assertEquals($expected, $value);

  }

  /**
   * Test the password
   */
  public function testPassword(): void {

    $config = new MockCluster();

    $expected = 'pokertestvalue';

    $rv = $config->setPassword($expected);
    $this->assertTrue($rv);

    $value = $config->getPassword();

    $this->assertEquals($expected, $value);

  }

  /**
   * Test the server adding and random server functionality
   */
  public function testAddServer(): void {

    $expected = 'poker-test-server-name';
    $config = new MockCluster();
    $config->clearServers();

    $rv = $config->addServer($expected);
    $this->assertTrue($rv);

    $value = $config->getRandomServer();

    $this->assertEquals($expected, $value);

  }

  /**
   * Test the intitial state for the object
   */
  public function testIntitialState(): void {

    $config = new MockCluster();

    $this->assertEquals(MockCluster::TEST_USERNAME, $config->getUserName());
    $this->assertEquals(MockCluster::TEST_PASSWORD, $config->getPassword());
    $this->assertEquals(5, $config->getServerCount());
    $this->assertEquals(
      1,
      preg_match('/poker-mock-\d+/', $config->getRandomServer()),
    );

    // muck up the up the object
    $muckUsername = 'muckusername';
    $muckPassword = 'muckpassword';
    $muckServer = 'muckserver';

    $config->clearServers();
    $config->addServer($muckServer);
    $config->setUserName($muckUsername);
    $config->setPassword($muckPassword);

    $this->assertEquals($muckUsername, $config->getUserName());
    $this->assertEquals($muckPassword, $config->getPassword());
    $this->assertEquals(1, $config->getServerCount());
    $this->assertEquals($muckServer, $config->getRandomServer());

    // this call should take us back to initial state.
    $config->init();

    $this->assertEquals(MockCluster::TEST_USERNAME, $config->getUserName());
    $this->assertEquals(MockCluster::TEST_PASSWORD, $config->getPassword());
    $this->assertEquals(5, $config->getServerCount());
    $this->assertEquals(
      1,
      preg_match('/poker-mock-\d+/', $config->getRandomServer()),
    );

    // if we clear the servers random should return a empty string
    $config->clearServers();
    $this->assertEmpty($config->getRandomServer());

  }

  public function testClusterInitFailure(): void {

    $this->expectException(ClusterInitFailureException::class);
    $config = new BrokenClusterInit();

  }

  public function testClusterNoServersAdded(): void {

    $this->expectException(ClusterInitNoServersException::class);
    $config = new BrokenClusterNoServers();

  }

}
