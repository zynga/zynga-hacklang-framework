<?hh // strict

namespace Zynga\Framework\ReflectionCache\V1;

use Zynga\Framework\ReflectionCache\V1\ReflectionMethods;

use \ReflectionMethod;

class ReflectionMethodDocComments {
  private static Map<string, ?string> $_docComments = Map {};

  public static function createKey(string $className, string $methodName): string {
    return $className . '::' . $methodName;
  }

  public static function getDocComment(string $className, string $methodName): ?string {

    $docComment = null;
    $cacheKey   = self::createKey($className, $methodName);

    if ( self::$_docComments->containsKey($cacheKey) ) {
      $docComment = self::$_docComments->get($cacheKey);
      return $docComment;
    }

    $reflectionClass = ReflectionMethods::getReflection($className, $methodName);

    if ( $reflectionClass instanceof ReflectionMethod ) {

      $t_docComment = $reflectionClass->getDocComment();

      if ( is_string($t_docComment) ) {
        $docComment = $t_docComment;
      }

    }

    self::$_docComments->set($cacheKey, $docComment);

    return $docComment;

  }

}
