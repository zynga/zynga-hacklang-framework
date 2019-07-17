<?hh // strict

namespace Zynga\Framework\Dynamic\V1;

use Zynga\Framework\Dynamic\V1\Exceptions\UnableToFindClassException;
use Zynga\Framework\Dynamic\V1\Exceptions\MissingRequiredParametersException;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\ReflectionCache\V1\ReflectionClasses;

use \ReflectionClass;
use \ReflectionMethod;

/**
 * Do you need to instantiate an object dynamically at runtime?
 * Are you trying to write good programming paradigms?
 * Then this is the class for you! This is a helper factory intended
 * to dynamically stand up objects at runtime where the type may change.
 */
class DynamicClassCreation {

  public static function doesClassExist(string $name): bool {
    if (class_exists($name, true) === true) {
      return true;
    }
    return false;
  }

  public static function doesClassImplementInterface(
    string $className,
    string $interfaceName,
  ): bool {

    $reflected = ReflectionClasses::getReflection($className);

    if (!$reflected instanceof ReflectionClass) {
      throw new UnableToFindClassException(
        'class='.$className.' - unable to reflect',
      );
    }

    return in_array($interfaceName, $reflected->getInterfaceNames());
  }

  public static function isClassAbstract(string $className): bool {
    if (DynamicClassCreation::doesClassExist($className) === false) {
      throw new UnableToFindClassException('class='.$className);
    }

    $reflected = ReflectionClasses::getReflection($className);

    if (!$reflected instanceof ReflectionClass) {
      throw new UnableToFindClassException(
        'class='.$className.' - unable to reflect',
      );
    }

    return $reflected->isAbstract();
  }

  public static function createClassByNameGeneric<Tv>(
    string $name,
    Vector<mixed> $params,
  ): Tv {

    try {

      $obj = null;

      $reflected = ReflectionClasses::getReflection($name);

      if (!$reflected instanceof ReflectionClass) {
        throw new UnableToFindClassException(
          'class='.$name.' - unable to reflect',
        );
      }

      $constructor = $reflected->getConstructor();

      $requiredParamCount = 0;
      $paramCount = $params->count();

      if ($constructor instanceof ReflectionMethod) {
        $requiredParamCount = $constructor->getNumberOfRequiredParameters();
      }

      if ($requiredParamCount > 0 && $paramCount < $requiredParamCount) {
        throw new MissingRequiredParametersException(
          'string='.
          $name.
          ' got='.
          $paramCount.
          ' expected='.
          $requiredParamCount.
          ' params='.
          json_encode($params),
        );
      }

      if ($params->count() > 0) {
        $obj = $reflected->newInstanceArgs($params->toArray());
      } else {
        $obj = $reflected->newInstance();
      }

      return $obj;

    } catch (Exception $e) {
      throw $e;
    }
  }

  /**
   * You should only use this if you have no idea what the runtime type
   * being generated is. If you do, use @see DynamicClassCreation::createClassByName instead.
   */
  public static function createClassByName(
    string $name,
    Vector<mixed> $params,
  ): mixed {
    return static::createClassByNameGeneric($name, $params);
  }

}
