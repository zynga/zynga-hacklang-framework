<?hh // strict

namespace Zynga\Framework\Dynamic\V1;

use Zynga\Framework\Dynamic\V1\Exceptions\UnableToFindClassException;
use Zynga\Framework\Exception\V1\Exception;

use \ReflectionClass;
use \ReflectionMethod;

class DynamicMethodCall {

  public static function callMethod(string $className, string $method, bool $allowMissingClass = false): mixed {

    if ( class_exists($className, true) != true ){

      // toss a exception at the user if the class is not autoloadable.
      if ( $allowMissingClass === false ) {
        error_log('JEO failed to findClassName=' . $className);
        throw new UnableToFindClassException('Failed to find className=' . $className);
      }

      // sorry no class to work with.
      return null;

    }

    try {

      // pull a reflection of the class
      $refClass = new ReflectionClass($className);

      // pull a reflection of the method
      $refMethod = $refClass->getMethod($method);

      // run the targeted item
      return $refMethod->invoke(null);

    } catch ( Exception $e ) {
      throw $e;
    }

  }

}
