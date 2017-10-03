<?hh // strict

namespace Zynga\Framework\Testing\TestCase\V2\Config;

class Manager {

  private static Map<string, string> $_disabledTests = Map {};
  private static Map<string, string> $_disabledTestPatterns = Map {};

  public static function enableTest(string $className): bool {

    if (self::$_disabledTests->containsKey($className) === true) {
      self::$_disabledTests->removeKey($className);
      return true;
    }

    return false;

  }

  public static function enableTestPattern(string $pattern): bool {
    if (self::$_disabledTestPatterns->containsKey($pattern) === true) {
      self::$_disabledTestPatterns->removeKey($pattern);
      return true;
    }
    return false;
  }

  public static function disableTestPattern(
    string $pattern,
    string $reason,
  ): bool {

    if (self::$_disabledTestPatterns->containsKey($pattern) !== true) {
      self::$_disabledTestPatterns[$pattern] = $reason;
      return true;
    }

    return false;

  }

  public static function disableTest(string $className, string $reason): bool {

    if (self::$_disabledTests->containsKey($className) !== true) {
      self::$_disabledTests[$className] = $reason;
      return true;
    }

    return false;
  }

  public static function isTestEnabled(string $className): bool {

    // --
    // error_log('className=' . $className);
    // --
    foreach (self::$_disabledTestPatterns as $pattern => $reason) {
      if (preg_match($pattern, $className)) {
        // error_log('className=' . $className . ' patternMatch=' . $pattern);
        return false;
      }
    }

    if (self::$_disabledTests->containsKey($className) === true) {
      return false;
    }

    // default to enabled.
    return true;

  }

  public static function getReason(string $className): string {

    foreach (self::$_disabledTestPatterns as $pattern => $reason) {
      if (preg_match($pattern, $className)) {
        // error_log('className=' . $className . ' patternMatch=' . $pattern);
        return self::$_disabledTestPatterns[$pattern];
      }
    }

    if (self::$_disabledTests->containsKey($className) === true) {
      return self::$_disabledTests[$className];
    }
    return '';
  }

}
