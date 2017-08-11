<?hh //strict

namespace Zynga\Framework\Testing\BootStrap\V2;

use Zynga\Framework\Environment\CodePath\V1\CodePath;
use Zynga\Framework\Testing\TestCase\V2\Base as ZyngaTestCase;
use Zynga\Framework\Testing\BootStrap\V2\BootStrap;

class BootStrapTest extends ZyngaTestCase {

  /**
   * Do we return the right base path for our error log
   */
  public function testValidFileSystemRoot(): void {
    $expected = CodePath::getRoot().'/tests';
    $value = BootStrap::getFileSystemRoot();
    $this->assertEquals($expected, $value);
  }

  /**
   * Do we return the right path for our error log
   */
  public function testValidGetTestErrorLog(): void {
    $expected = CodePath::getRoot().'/tests/php.log';
    $value = BootStrap::getTestErrorLog();
    $this->assertEquals($expected, $value);
  }

  /**
   * Test all the bootstrap settings we expect stood up for our testing framework.
   */
  public function testValidInitTestErrorLog(): void {

    $file = BootStrap::getTestErrorLog();

    if (is_file($file)) {
      unlink($file);
    }

    BootStrap::initTestErrorLog();

    $this->assertFileExists($file);
    $this->assertEquals(BootStrap::getTestErrorLog(), ini_get('error_log'));
    $this->assertEquals(E_ALL, ini_get('error_reporting'));
    $this->assertEquals(true, ini_get('log_errors'));

  }

}
