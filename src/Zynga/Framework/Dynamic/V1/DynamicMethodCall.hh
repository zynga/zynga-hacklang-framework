<?hh // strict

namespace Zynga\Framework\Dynamic\V1;

use Zynga\Framework\Dynamic\V1\Exceptions\UnableToFindClassException;
use Zynga\Framework\Dynamic\V1\Exceptions\UnableToFindMethodException;
use Zynga\Framework\Dynamic\V1\Exceptions\UnableToReflectClassException;
use Zynga\Framework\Dynamic\V1\Exceptions\MissingRequiredParametersException;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\ReflectionCache\V1\ReflectionClasses;

use \ReflectionClass;
use \ReflectionMethod;

class DynamicMethodCall {

  public static function callMethod(
    string $className,
    string $method,
    Vector<mixed> $params,
    bool $allowMissingClass = false,
  ): mixed {

    try {

      // pull a reflection of the class
      $refClass = ReflectionClasses::getReflection($className);

      if (!$refClass instanceof ReflectionClass) {
        throw new UnableToReflectClassException(
          'Failed to reflect className='.$className,
        );
      }

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

    } catch (UnableToReflectClassException $e) {
      if ($allowMissingClass === true) {
        return null;
      }
      throw $e;
    } catch (Exception $e) {
      throw $e;
    }

  }

}
