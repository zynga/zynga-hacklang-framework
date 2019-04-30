<?hh //strict

namespace Zynga\Framework\ShardedDatabase\V3\Config;

use Zynga\Framework\Database\V2\Exceptions\NoPasswordException;
use Zynga\Framework\Database\V2\Exceptions\NoPortProvidedException;
use Zynga\Framework\Database\V2\Exceptions\NoUserNameException;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\ShardedDatabase\V3\Exceptions\ShardsInitNoServersException;

use Zynga\Framework\ShardedDatabase\V3\Test\UserSharded\Config\Mock\Base\Valid as MockValid;
use Zynga\Framework\ShardedDatabase\V3\Test\UserSharded\Config\Mock\Base\NoUserName as MockNoUserName;
use Zynga\Framework\ShardedDatabase\V3\Test\UserSharded\Config\Mock\Base\NoPassword as MockNoPassword;
use Zynga\Framework\ShardedDatabase\V3\Test\UserSharded\Config\Mock\Base\NoPort as MockNoPort;
use Zynga\Framework\ShardedDatabase\V3\Test\UserSharded\Config\Mock\Base\EmptyConfig as MockEmptyConfig;

class BaseTest extends TestCase {

  public function testConstruct(): void {
    $this->expectException(ShardsInitNoServersException::class);
    $obj = new MockEmptyConfig();
  }

  /**
   * Test the lack of a username is emitting exceptions
   */
  public function testNoUsername(): void {
    $this->expectException(NoUserNameException::class);
    $obj = new MockNoUserName();
  }

  /**
   * Test the lack of a password is emitting exceptions
   */
  public function testNoPassword(): void {
    $this->expectException(NoPasswordException::class);
    $obj = new MockNoPassword();
  }

  /**
   * Test the lack of a port is emitting exceptions.
   */
  public function testNoPort(): void {
    $this->expectException(NoPortProvidedException::class);
    $obj = new MockNoPort();
  }

  public function testDatabaseFunctions(): void {
    $expectedDatabase = 'oratest';

    $conf = new MockValid();

    $this->assertEquals('', $conf->getCurrentDatabase());

    $conf->setCurrentDatabase($expectedDatabase);

    $this->assertEquals($expectedDatabase, $conf->getCurrentDatabase());

  }

}
