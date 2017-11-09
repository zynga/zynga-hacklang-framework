<?hh // strict

namespace Zynga\Framework\Dynamic\V1;

use Zynga\Framework\Dynamic\V1\Exceptions\UnableToFindClassException;
use Zynga\Framework\Dynamic\V1\Exceptions\MissingRequiredParametersException;
use Zynga\Framework\Exception\V1\Exception;

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

  public static function createClassByNameGeneric<Tv>(
    string $name,
    Vector<mixed> $params,
  ): Tv {
    if (class_exists($name, true) !== true) {
      throw new UnableToFindClassException('class='.$name);
    }

    try {

      $obj = null;

      $reflected = new ReflectionClass($name);

      $constructor = $reflected->getConstructor();

      $constructorParamCount = 0;

      if ($constructor instanceof ReflectionMethod) {
        $constructorParamCount =
          $constructor->getNumberOfRequiredParameters();
      }

      if ($constructorParamCount != $params->count()) {
        throw new MissingRequiredParametersException(
          'string='.
          $name.
          ' got='.
          count($params).
          ' expected='.
          $constructorParamCount.
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
