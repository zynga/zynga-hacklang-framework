<?hh //strict

namespace Zynga\Framework\Database\V2\Config;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Database\V2\Test\Config\Mock\Base\Valid as MockValid;
use
  Zynga\Framework\Database\V2\Test\Config\Mock\Base\NoUserName as MockNoUserName
;
use
  Zynga\Framework\Database\V2\Test\Config\Mock\Base\NoPassword as MockNoPassword
;
use Zynga\Framework\Database\V2\Test\Config\Mock\Base\NoPort as MockNoPort;
use
  Zynga\Framework\Database\V2\Test\Config\Mock\Base\EmptyConfig as MockEmptyConfig
;

class BaseTest extends TestCase {

  /**
   * Test the object stands up without any values
   * @expectedException Zynga\Framework\Database\V2\Exceptions\NoUserNameException
   */
  public function testConstruct(): void {
    $obj = new MockEmptyConfig();
  }

  /**
   * Test the lack of a username is emitting exceptions
   * @expectedException Zynga\Framework\Database\V2\Exceptions\NoUserNameException
   */
  public function testNoUsername(): void {
    $obj = new MockNoUserName();
  }

  /**
   * Test the lack of a password is emitting exceptions
   * @expectedException Zynga\Framework\Database\V2\Exceptions\NoPasswordException
   */
  public function testNoPassword(): void {
    $obj = new MockNoPassword();
  }

  /**
   * Test the lack of a port is emitting exceptions
   * @expectedException Zynga\Framework\Database\V2\Exceptions\NoPortProvidedException
   */
  public function testNoPort(): void {
    $obj = new MockNoPort();
  }

  public function testDatabaseFunctions(): void {
    $expectedDatabase = 'oratest';
    $expectedServer = '';
    $expectedPort = 123;

    $conf = new MockValid();

    $this->assertEquals('', $conf->getCurrentDatabase());

    $conf->setCurrentDatabase($expectedDatabase);

    $this->assertEquals($expectedDatabase, $conf->getCurrentDatabase());
    $this->assertEquals($expectedPort, $conf->getPort());
    $this->assertEquals($expectedServer, $conf->getCurrentServer());

  }

}
