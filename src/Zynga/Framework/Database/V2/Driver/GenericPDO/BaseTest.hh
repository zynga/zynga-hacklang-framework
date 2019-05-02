<?hh //strict

namespace Zynga\Framework\Database\V2\Driver\GenericPDO;

use Zynga\Framework\Database\V2\Config\Test\Mysql\Dev as MockConfig;
use
  Zynga\Framework\Database\V2\Config\Test\Mysql\ReadOnly\Dev as MockReadOnlyConfig
;
use Zynga\Framework\Database\V2\Driver\GenericPDO\Base as BaseDriver;
use Zynga\Framework\Database\V2\Exceptions\ConnectionIsReadOnly;
use Zynga\Framework\Database\V2\Exceptions\ConnectionGoneAwayException;
use Zynga\Framework\Database\V2\Exceptions\QueryFailedException;
use Zynga\Framework\Database\V2\Interfaces\QuoteInterface;
use Zynga\Framework\Database\V2\Interfaces\TransactionInterface;
use Zynga\Framework\Database\V2\Interfaces\ResultSetInterface;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class BaseTest extends TestCase {

  public function testConfig(): void {
    $config = new MockConfig();
    $this->assertEquals(false, $config->isDatabaseReadOnly());
    $this->assertEquals('GenericPDO\Base', $config->getDriver());
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

  public function test_ReadOnlyConfig(): void {
    $config = new MockReadOnlyConfig();
    $this->assertTrue($config->isDatabaseReadOnly());
    $this->assertEquals('GenericPDO\Base', $config->getDriver());
    $this->assertEquals(
      'mysql:host=localhost;user=zframework;password=i-am-a-walrus;port=3306;dbname=phpunit;',
      $config->getConnectionString(),
    );
  }

  public function testQuery_ConnectionReadOnlyButHasDML(): void {

    $config = new MockReadOnlyConfig();
    $driver = new Base($config);

    // No: TRUNCATE, DELETE, UPDATE
    $this->expectException(ConnectionIsReadOnly::class);
    $driver->query('TRUNCATE test_table;');
    $this->expectException(ConnectionIsReadOnly::class);
    $driver->query('DELETE FROM test_table;');
    $this->expectException(ConnectionIsReadOnly::class);
    $driver->query('UPDATE TABLE test_table;');

  }

  public function testQuery_SimpleQuery(): void {
    $config = new MockConfig();
    $driver = new Base($config);
    $sth = $driver->query('SELECT 1;');
    $this->assertClassImplements(ResultSetInterface::class, $sth);
  }

  public function testQuery_BrokenConnection(): void {
    $config = new MockConfig();
    $config->setPassword('invalid-password');
    $driver = new Base($config);
    $this->expectException(ConnectionGoneAwayException::class);
    $driver->query('SELECT 1');
  }

  public function testQuery_BrokenQuery(): void {
    $config = new MockConfig();
    $driver = new Base($config);
    $this->expectException(QueryFailedException::class);
    $sth = $driver->query('SELECT 1 "bad-query, needs comma and a quote');
  }

  public function testNativeQuoteString_BrokenConnection(): void {
    $config = new MockConfig();
    $config->setPassword('invalid-password');
    $driver = new Base($config);
    $this->expectException(ConnectionGoneAwayException::class);
    $driver->nativeQuoteString('a-value');
  }

  public function testNativeQuoteString_Valid(): void {
    $config = new MockConfig();
    $driver = new Base($config);
    $this->assertEquals("'a-value'", $driver->nativeQuoteString('a-value'));
  }

}
