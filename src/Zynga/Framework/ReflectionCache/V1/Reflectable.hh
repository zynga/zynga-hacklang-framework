<?hh // strict

namespace Zynga\Framework\ReflectionCache\V1;

class Reflectable {

  public static function isClassReflectable(string $className): bool {

    if ( class_exists($className) ||
         interface_exists($className) ||
         trait_exists($className) ) {
           return true;
    }

    return false;

  }

  public static function isMethodReflectable(string $className, string $methodName): bool {
    if ( method_exists($className, $methodName) === true ) {
      return true;
    }
    return false;
  }

}
