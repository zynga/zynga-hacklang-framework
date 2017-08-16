<?hh // strict

namespace Zynga\Framework\Dynamic\V1;

use Zynga\Framework\Dynamic\V1\Exceptions\UnableToFindClassException;
use Zynga\Framework\Dynamic\V1\Exceptions\MissingRequiredParametersException;
use Zynga\Framework\Exception\V1\Exception;

use \ReflectionClass;
use \ReflectionMethod;

class DynamicClassCreation {

  public static function createClassByName(
    string $name,
    Vector<mixed> $params,
  ): mixed {

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

}
