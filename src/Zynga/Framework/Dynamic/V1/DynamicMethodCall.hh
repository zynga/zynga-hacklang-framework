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

  public static function callMethodOnObject(
    mixed $object,
    string $method,
    Vector<mixed> $params,
    bool $allowMissingClass = false,
    bool $allowMissingMethod = false,
  ): mixed {

    try {

      if (!is_object($object)) {
        if ($allowMissingClass == true) {
          return null;
        }
        throw new UnableToReflectClassException(
          'Failed to reflect non-object object='.json_encode($object),
        );
      }

      $className = get_class($object);

      // pull a reflection of the class
      $refClass = ReflectionClasses::getReflection($object);

      if (!$refClass instanceof ReflectionClass) {
        if ($allowMissingClass == true) {
          return null;
        }
        throw new UnableToReflectClassException(
          'Failed to reflect className='.$className,
        );
      }

      if ($refClass->hasMethod($method) !== true) {
        if ($allowMissingMethod == true) {
          return null;
        }
        throw new UnableToFindMethodException(
          'class='.$className.' method='.$method,
        );
      }

      // pull a reflection of the method
      $refMethod = $refClass->getMethod($method);

      // pull the parameter count off the method we are calling.
      $requiredParamCount = $refMethod->getNumberOfRequiredParameters();
      $paramCount = $params->count();

      // JEO: Allow for optional parameters at the end of the function.
      if ($requiredParamCount > 0 && $paramCount < $requiredParamCount) {

        throw new MissingRequiredParametersException(
          'className='.
          $className.
          ' got='.
          $paramCount.
          ' expected='.
          $requiredParamCount.
          ' params='.
          json_encode($params),
        );
      }

      // run the targeted item
      $paramArray = $params->toArray();

      return $refMethod->invokeArgs($object, $paramArray);

    } catch (UnableToReflectClassException $e) {
      throw $e;
    } catch (Exception $e) {
      throw $e;
    }

  }

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
      $requiredParamCount = $refMethod->getNumberOfRequiredParameters();
      $paramCount = $params->count();

      if ($requiredParamCount > 0 && $paramCount < $requiredParamCount) {
        throw new MissingRequiredParametersException(
          'className='.
          $className.
          ' got='.
          $paramCount.
          ' expected='.
          $requiredParamCount.
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
