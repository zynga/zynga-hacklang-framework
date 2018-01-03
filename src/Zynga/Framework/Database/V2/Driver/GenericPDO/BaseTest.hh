<?hh //strict

namespace Zynga\Framework\Database\V2\Driver\GenericPDO;

use Zynga\Framework\Database\V2\Config\Test\Mysql\Dev as MockConfig;
use Zynga\Framework\Database\V2\Driver\GenericPDO\Base as BaseDriver;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class BaseTest extends TestCase {

  public function testConfig(): void {
    $config = new MockConfig();
    $this->assertEquals(false, $config->isDatabaseReadOnly());
    $this->assertEquals('GenericPDO', $config->getDriver());
  }

  public function testGetQuoter(): void {
    $driver = new Base(new MockConfig());
    $this->assertTrue($driver->getQuoter() instanceof QuoteInterface);
  }

  public function testGetTransaction(): void {
    $driver = new Base(new MockConfig());
    $this->assertTrue(
      $driver->getTransaction() instanceof TransactionInterface,
    );
  }

  public function testSetIsConnected(): void {
    $driver = new Base(new MockConfig());
    $this->assertTrue($driver->setIsConnected(true));
  }

  public function testGetIsConnected(): void {
    $driver = new Base(new MockConfig());
    $driver->setIsConnected(true);
    $this->assertTrue($driver->getIsConnected());
  }

  public function testConnectWithStringWhileConnected(): void {
    $driver = new Base(new MockConfig());
    $driver->setIsConnected(true);
    $this->assertTrue($driver->connectWithString(''));
  }

  public function testConnectWithStringWhileDisconnected(): void {
    $config = new MockConfig();
    $driver = new Base($config);
    $driver->setIsConnected(false);
    $this->assertTrue(
      $driver->connectWithString($config->getConnectionString()),
    );
  }

  public function testConnect(): void {
    $config = new MockConfig();
    $driver = new Base($config);
    $this->assertTrue($driver->connect());
  }

  public function testConnect_BrokenData(): void {
    $config = new MockConfig();
    $config->setPassword('my-voice-is-my-password-verify-me');
    $driver = new BaseDriver($config);
    $this->assertFalse($driver->connect());
    $this->assertTrue($driver->hadError());
  }

  public function testDisconnect(): void {
    $config = new MockConfig();
    $driver = new Base($config);
    $this->assertTrue($driver->disconnect());
  }

  public function testOnDriverConnectionChangeDoesNotAlterConnected(): void {
    $config = new MockConfig();
    $driver = new Base($config);
    $driver->setIsConnected(true);
    $this->assertTrue($driver->getIsConnected());
    $driver->onDriverConnectionChange(true, false);
    $this->assertTrue($driver->getIsConnected());
  }

  public function testDefaultHadError(): void {
    $config = new MockConfig();
    $driver = new Base($config);
    $this->assertFalse($driver->hadError());
  }

  public function testDefaultGetLastError(): void {
    $config = new MockConfig();
    $driver = new Base($config);
    $this->assertEquals(0, strlen($driver->getLastError()));
  }
}
