<?hh //strict

namespace Zynga\Environment\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Environment\DevelopmentMode\V1\DevelopmentMode;
use Zynga\Framework\Environment\DevelopmentMode\V1\Detector;
use Zynga\Framework\Environment\DevelopmentMode\V1\Interfaces\DetectorInterface;

class Mock_DevDetector implements DetectorInterface {
  public function isDevelopment(): bool {
    return true;
  }
  public function isStaging(): bool {
    return false;
  }
  public function isProduction(): bool {
    return false;
  }
}

class Mock_StageDetector implements DetectorInterface {
  public function isDevelopment(): bool {
    return false;
  }
  public function isStaging(): bool {
    return true;
  }
  public function isProduction(): bool {
    return false;
  }
}

class Mock_ProdDetector implements DetectorInterface {
  public function isDevelopment(): bool {
    return false;
  }
  public function isStaging(): bool {
    return false;
  }
  public function isProduction(): bool {
    return true;
  }
}

class Mock_UnknownDetector implements DetectorInterface {
  public function isDevelopment(): bool {
    return false;
  }
  public function isStaging(): bool {
    return false;
  }
  public function isProduction(): bool {
    return false;
  }
}

class DevelopmentModeTest extends TestCase {

  public function testInvalidSet(): void {
    $this->assertFalse(DevelopmentMode::setMode(12345));
  }

  public function testNormalStandup(): void {

    // reset to initial state
    DevelopmentMode::reset();

    // are we at init state?
    $this->assertEquals(DevelopmentMode::INIT, DevelopmentMode::getMode());

    // Do detection
    // --
    // JEO: Our assertion that detection works is broken until we deploy the
    // /var/opt/files on prod.
    // disabling for now.
    // --
    DevelopmentMode::detect();

    $this->assertTrue(DevelopmentMode::getMode() > DevelopmentMode::INIT);

    // cleanup after ourselves
    DevelopmentMode::reset();

    DevelopmentMode::setDetector(new Detector());
    DevelopmentMode::detect();

  }

  public function testDevDetect(): void {

    $detector = new Mock_DevDetector();

    // we have to reset our state as development mode may have already been used.
    DevelopmentMode::reset();

    // are we at init state?
    $this->assertEquals(DevelopmentMode::INIT, DevelopmentMode::getMode());

    // Dev mode mock goes in now!
    DevelopmentMode::setDetector($detector);

    // Do detection
    $this->assertTrue(DevelopmentMode::detect());

    // dev mode test.
    $this->assertTrue(DevelopmentMode::setMode(DevelopmentMode::DEV));
    $this->assertEquals(DevelopmentMode::DEV, DevelopmentMode::getMode());
    $this->assertTrue(DevelopmentMode::isDevelopment());
    $this->assertFalse(DevelopmentMode::isStaging());
    $this->assertFalse(DevelopmentMode::isProduction());
    $this->assertEquals('Dev', DevelopmentMode::getModeAsString());

    // Clean up after ourselves.
    DevelopmentMode::reset();
    DevelopmentMode::setDetector(new Detector());
    DevelopmentMode::detect();

  }

  public function testStaqeDetect(): void {

    $detector = new Mock_StageDetector();

    // we have to reset our state as development mode may have already been used.
    DevelopmentMode::reset();

    // are we at init state?
    $this->assertEquals(DevelopmentMode::INIT, DevelopmentMode::getMode());

    // Dev mode mock goes in now!
    DevelopmentMode::setDetector($detector);

    // Do detection
    $this->assertTrue(DevelopmentMode::detect());

    // Do detection
    $this->assertTrue(DevelopmentMode::detect());

    // dev mode test.
    $this->assertTrue(DevelopmentMode::setMode(DevelopmentMode::STAGING));
    $this->assertEquals(DevelopmentMode::STAGING, DevelopmentMode::getMode());
    $this->assertFalse(DevelopmentMode::isDevelopment());
    $this->assertTrue(DevelopmentMode::isStaging());
    $this->assertFalse(DevelopmentMode::isProduction());
    $this->assertEquals('Staging', DevelopmentMode::getModeAsString());

    // Clean up after ourselves.
    DevelopmentMode::reset();
    DevelopmentMode::setDetector(new Detector());
    DevelopmentMode::detect();

  }

  public function testProdDetect(): void {

    $detector = new Mock_ProdDetector();

    // we have to reset our state as development mode may have already been used.
    DevelopmentMode::reset();

    // are we at init state?
    $this->assertEquals(DevelopmentMode::INIT, DevelopmentMode::getMode());

    // Dev mode mock goes in now!
    DevelopmentMode::setDetector($detector);

    // Do detection
    $this->assertTrue(DevelopmentMode::detect());

    // dev mode test.
    $this->assertTrue(DevelopmentMode::setMode(DevelopmentMode::PRODUCTION));
    $this->assertEquals(
      DevelopmentMode::PRODUCTION,
      DevelopmentMode::getMode(),
    );
    $this->assertFalse(DevelopmentMode::isDevelopment());
    $this->assertFalse(DevelopmentMode::isStaging());
    $this->assertTrue(DevelopmentMode::isProduction());
    $this->assertEquals('Production', DevelopmentMode::getModeAsString());

    // Clean up after ourselves.
    DevelopmentMode::reset();
    DevelopmentMode::setDetector(new Detector());
    DevelopmentMode::detect();

  }

  public function testUnknownDetect(): void {

    $detector = new Mock_UnknownDetector();

    // we have to reset our state as development mode may have already been used.
    DevelopmentMode::reset();

    // are we at init state?
    $this->assertEquals(DevelopmentMode::INIT, DevelopmentMode::getMode());

    // Dev mode mock goes in now!
    DevelopmentMode::setDetector($detector);

    // Do detection
    $this->assertFalse(DevelopmentMode::detect());

    // dev mode test.
    $this->assertEquals(DevelopmentMode::UNKNOWN, DevelopmentMode::getMode());
    $this->assertFalse(DevelopmentMode::isDevelopment());
    $this->assertFalse(DevelopmentMode::isStaging());
    $this->assertFalse(DevelopmentMode::isProduction());
    $this->assertEquals('Unknown', DevelopmentMode::getModeAsString());

    // Clean up after ourselves.
    DevelopmentMode::reset();
    DevelopmentMode::setDetector(new Detector());
    DevelopmentMode::detect();

  }

}
