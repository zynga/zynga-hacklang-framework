<?hh // strict

namespace Zynga\Framework\Environment\DevelopmentMode\V1;

use
  Zynga\Framework\Environment\DevelopmentMode\V1\Interfaces\DetectorInterface
;
use Zynga\Framework\Environment\DevelopmentMode\V1\Detector;

class DevelopmentMode {

  const int INIT = 0;
  const int UNKNOWN = 2;
  const int DEV = 4;
  const int STAGING = 8;
  const int PRODUCTION = 16;

  private static int $_mode = self::INIT;
  private static ?DetectorInterface $_detector = null;

  public static function reset(): void {
    self::$_mode = self::INIT;
    self::$_detector = null;
  }

  public static function getModeAsString(): string {

    if (self::$_mode === self::DEV) {
      return 'Dev';
    } else if (self::$_mode === self::STAGING) {
      return 'Staging';
    } else if (self::$_mode === self::PRODUCTION) {
      return 'Production';
    }

    return 'Unknown';

  }

  public static function getMode(): int {
    return self::$_mode;
  }

  public static function setMode(int $mode): bool {

    if ($mode === self::DEV ||
        $mode === self::STAGING ||
        $mode === self::PRODUCTION ||
        $mode === self::UNKNOWN) {
      self::$_mode = $mode;
      return true;
    }

    return false;

  }

  public static function setDetector(DetectorInterface $obj): bool {
    self::$_detector = $obj;
    return true;
  }

  public static function detect(): bool {

    // has detection already happened?
    if (self::$_mode !== self::INIT) {
      return true;
    }

    if (self::$_detector === null) {
      self::$_detector = new Detector();
    }

    $detector = self::$_detector;

    // is a dev env?
    if ($detector->isDevelopment() === true) {
      return self::setMode(self::DEV);
    }

    // is it a staging environment?
    if ($detector->isStaging() === true) {
      return self::setMode(self::STAGING);
    }

    // is it a production environment?
    if ($detector->isProduction() === true) {
      return self::setMode(self::PRODUCTION);
    }

    // we don't know whats going on.
    self::setMode(self::UNKNOWN);
    return false;

  }

  public static function isDevelopment(): bool {

    if (self::$_mode === self::DEV) {
      return true;
    }

    return false;

  }

  public static function isStaging(): bool {

    if (self::$_mode === self::STAGING) {
      return true;
    }

    return false;

  }

  public static function isProduction(): bool {

    if (self::$_mode === self::PRODUCTION) {
      return true;
    }

    return false;

  }

}
