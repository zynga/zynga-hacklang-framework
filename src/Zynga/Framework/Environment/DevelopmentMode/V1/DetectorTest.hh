<?hh //strict

namespace Zynga\Framework\Environment\DevelopmentMode\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Environment\DevelopmentMode\V1\Detector;

class DetectorTest extends TestCase {

  public function testDevelopmentMode(): void {

    // --
    // This test only works on development hosts.
    // --
    $isBox = is_file('/opt/zynga/is_development');

    $obj = new Detector();
    $mode = $obj->isDevelopment();

    if ($isBox === true) {
      $this->assertTrue($mode);
      $this->assertFalse($obj->isDevelopment('/tmp/no-such-file'));
    } else {
      $this->assertFalse($mode);
      $this->assertTrue($obj->isDevelopment('/etc/hosts'));
    }

  }

  public function testStagingMode(): void {

    // --
    // This test only works on staging hosts.
    // --
    $isBox = is_file('/opt/zynga/is_staging');

    $obj = new Detector();
    $mode = $obj->isStaging();

    if ($isBox === true) {
      $this->assertTrue($mode);
      $this->assertFalse($obj->isStaging('/tmp/no-such-file'));
    } else {
      $this->assertFalse($mode);
      $this->assertTrue($obj->isStaging('/etc/hosts'));
    }

  }

  public function testProductionMode(): void {

    // --
    // This test only works on production hosts.
    // --
    $isBox = is_file('/opt/zynga/is_production');

    $obj = new Detector();
    $mode = $obj->isProduction();

    if ($isBox === true) {
      $this->assertTrue($mode);
      $this->assertFalse($obj->isProduction('/tmp/no-such-file'));
    } else {
      $this->assertFalse($mode);
      $this->assertTrue($obj->isProduction('/etc/hosts'));
    }

  }

}
