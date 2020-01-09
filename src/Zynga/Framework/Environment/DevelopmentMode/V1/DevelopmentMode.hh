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
      self::$_mode = self::DEV;
      return true;
    }

    // is it a staging environment?
    if ($detector->isStaging() === true) {
      self::$_mode = self::STAGING;
      return true;
    }

    // is it a production environment?
    if ($detector->isProduction() === true) {
      self::$_mode = self::PRODUCTION;
      return true;
    }

    // we don't know whats going on.
    self::$_mode = self::UNKNOWN;
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
