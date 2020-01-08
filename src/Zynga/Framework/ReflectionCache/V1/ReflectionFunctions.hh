<?hh // strict

namespace Zynga\Framework\ReflectionCache\V1;

use \Exception;
use \ReflectionFunction;

class ReflectionFunctions {

  private static Map<string, ?ReflectionFunction> $_cache = Map {};

  public static function getReflection(string $functionName): ?ReflectionFunction {

    $reflected = null;
    $cacheKey = $functionName;

    if ( self::$_cache->containsKey($cacheKey) === true ) {
      $reflected = self::$_cache->get($cacheKey);
      return $reflected;
    }

    try {
      $reflected = new ReflectionFunction($cacheKey);
    } catch ( Exception $e ) {
      $reflected = null;
    }

    self::$_cache->set($cacheKey, $reflected);

    return $reflected;

  }

}
