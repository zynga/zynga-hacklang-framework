<?hh // strict

namespace Zynga\Framework\ReflectionCache\V1;

use \Exception;
use \ReflectionClass;

class ReflectionClasses {
  private static bool $shouldFail = false;
  private static string $classNameToFail = '';

  private static Map<string, ?ReflectionClass> $_classes = Map {};

  public static function getReflection(mixed $classOrName): ?ReflectionClass {
    $className = self::getClassName($classOrName);

    if (self::$shouldFail === true && self::$classNameToFail === $className) {
      return null;
    }

    try {

      $reflected = self::$_classes->get($className);

      if ($reflected instanceof ReflectionClass) {
        return $reflected;
      }

      // --
      // attempt to find the class in the interpreter, allow the autoloader to
      // pick up the class if needed.
      // --
      if (class_exists($className, true) !== true) {
        return null;
      }

      // The one and only ReflectionClass call in the stack.
      $reflected = new ReflectionClass($className);
    } catch (Exception $e) {
      $reflected = null;
    }

    self::$_classes->set($className, $reflected);

    return $reflected;

  }

  private static function getClassName(mixed $classOrName): string {
    if (is_string($classOrName)) {
      return $classOrName;
    }

    if (is_object($classOrName)) {
      return get_class($classOrName);
    }

    return strval($classOrName);
  }

  public static function enableFailure(mixed $classOrName): void {
    self::$shouldFail = true;
    self::$classNameToFail = self::getClassName($classOrName);
  }

  public static function disableFailure(): void {
    self::$shouldFail = false;
    self::$classNameToFail = '';
  }
}
