<?hh // strict

namespace Zynga\Framework\Dynamic\V1;

use Zynga\Framework\Dynamic\V1\Exceptions\UnableToFindClassException;
use Zynga\Framework\Dynamic\V1\Exceptions\UnableToFindMethodException;
use Zynga\Framework\Dynamic\V1\Exceptions\MissingRequiredParametersException;
use Zynga\Framework\Exception\V1\Exception;

use \ReflectionClass;
use \ReflectionMethod;

class DynamicMethodCall {

  public static function callMethod(
    string $className,
    string $method,
    Vector<mixed> $params,
    bool $allowMissingClass = false,
  ): mixed {

    if (class_exists($className, true) != true) {

      // toss a exception at the user if the class is not autoloadable.
      if ($allowMissingClass === false) {
        throw new UnableToFindClassException(
          'Failed to find className='.$className,
        );
      }

      // sorry no class to work with.
      return null;

    }

    try {

      // pull a reflection of the class
      $refClass = new ReflectionClass($className);

      if ($refClass->hasMethod($method) !== true) {
        throw new UnableToFindMethodException(
          'class='.$className.' method='.$method,
        );
      }

      // pull a reflection of the method
      $refMethod = $refClass->getMethod($method);

      // pull the parameter count off the method we are calling.
      $paramCount = $refMethod->getNumberOfRequiredParameters();

      if ($paramCount != $params->count()) {
        throw new MissingRequiredParametersException(
          'className='.
          $className.
          ' got='.
          $params->count().
          ' expected='.
          $paramCount.
          ' params='.
          json_encode($params),
        );
      }

      // run the targeted item
      $paramArray = $params->toArray();

      if ($paramCount > 0) {
        return $refMethod->invokeArgs(null, $paramArray);
      } else {
        return $refMethod->invoke(null);
      }

    } catch (Exception $e) {
      throw $e;
    }

  }

}
