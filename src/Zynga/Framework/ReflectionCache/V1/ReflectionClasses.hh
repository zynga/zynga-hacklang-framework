<?hh // strict

namespace Zynga\Framework\ReflectionCache\V1;

use \Exception;
use \ReflectionClass;

class ReflectionClasses {

  private static Map<string, ?ReflectionClass> $_classes = Map {};

  public static function getReflection(mixed $classOrName): ?ReflectionClass {

    $className = '';

    if ( is_object($classOrName) ) {
      $className = get_class($classOrName);
    } else {
      $className = strval($classOrName);
    }

    $reflected = null;

    if ( self::$_classes->containsKey($className) === true ) {
      $reflected = self::$_classes->get($className);
      return $reflected;
    }

    try {
      // The one and onyl ReflectionClass call in the stack.
      $reflected = new ReflectionClass($className);
    } catch ( Exception $e ) {
      $reflected = null;
    }

    self::$_classes->set($className, $reflected);

    return $reflected;

  }

}
