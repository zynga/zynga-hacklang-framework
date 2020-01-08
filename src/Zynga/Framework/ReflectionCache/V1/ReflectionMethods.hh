<?hh // strict

namespace Zynga\Framework\ReflectionCache\V1;

use \Exception;
use \ReflectionMethod;
use \ReflectionClass;

class ReflectionMethods {

  private static Map<string, Map <string, ?ReflectionMethod>> $_cache = Map {};
  private static Map<string, bool> $_lookedUpAllMethods = Map {};

  public static function getAllReflections(string $className): ?Vector<ReflectionMethod> {

    if ( self::$_lookedUpAllMethods->containsKey($className) &&
         self::$_lookedUpAllMethods->get($className) === true ) {

       $reflected = self::$_cache->get($className);

       if ( $reflected instanceof Map ) {
         return $reflected->values();
       }

       return null;
    }

    $reflected = ReflectionClasses::getReflection($className);

    if ( $reflected === null ) {
      return null;
    }


    if ( $reflected instanceof ReflectionClass ) {

      $methods = $reflected->getMethods();

      $classMethods = Map {};

      foreach ( $methods as $method ) {
        $methodName = $method->getName();
        $classMethods->set($methodName, $method);
      }

      self::$_lookedUpAllMethods->set($className, true);

      self::$_cache->set($className, $classMethods);

      $reflected = self::$_cache->get($className);

      if ( $reflected instanceof Map ) {
        return $reflected->values();
      }

      return null;

    }

    return null;

  }

  public static function getReflection(string $className, string $methodName): ?ReflectionMethod {

    $reflected = null;

    if ( self::$_cache->containsKey($className) === true ) {

      $classMethods = self::$_cache->get($className);

      if ( $classMethods !== null && $classMethods->containsKey($methodName) === true ) {
        $reflected = $classMethods->get($methodName);
        return $reflected;
      }

    }

    try {
      $reflected = new ReflectionMethod($className, $methodName);
    } catch ( Exception $e ) {
      $reflected = null;
    }

    if ( self::$_cache->containsKey($className) !== true ) {
      self::$_cache->set($className, Map {});
    }

    $methods = self::$_cache->get($className);

    if ( $methods !== null ) {
      $methods->set($methodName, $reflected);
    }

    return $reflected;

  }

}
