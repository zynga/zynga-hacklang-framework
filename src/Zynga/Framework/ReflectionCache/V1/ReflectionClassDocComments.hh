<?hh // strict

namespace Zynga\Framework\ReflectionCache\V1;

use Zynga\Framework\ReflectionCache\V1\ReflectionClasses;

use \ReflectionClass;

class ReflectionClassDocComments {
  private static Map<string, ?string> $_docComments = Map {};

  public static function getDocComment(string $className): ?string {

    $docComment = null;

    if ( self::$_docComments->containsKey($className) ) {
      $docComment = self::$_docComments->get($className);
      return $docComment;
    }

    $reflectionClass = ReflectionClasses::getReflection($className);

    if ( $reflectionClass instanceof ReflectionClass ) {

      $t_docComment = $reflectionClass->getDocComment();

      if ( is_string($t_docComment) ) {
        $docComment = $t_docComment;
      }

    }

    self::$_docComments->set($className, $docComment);

    return $docComment;

  }

}
