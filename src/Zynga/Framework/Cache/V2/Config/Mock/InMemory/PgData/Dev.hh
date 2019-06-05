<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\Mock\InMemory\PgData;

use Zynga\Framework\Cache\V2\Config\InMemory\Base as InMemoryBase;
use Zynga\Framework\Cache\V2\Exceptions\InvalidObjectForKeyCreationException;
use Zynga\Framework\PgData\V1\Exceptions\InvalidPrimaryKeyValueException;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use \Exception;

class Dev extends InMemoryBase {

  public function createKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {

    if ($obj instanceof PgRowInterface) {

      $pkValue = $obj->getPrimaryKeyTyped()->get();

      if ($pkValue == 0) {
        throw new InvalidPrimaryKeyValueException('id='.strval($pkValue));
      }

      $pk = strval($obj->getPrimaryKeyTyped()->get());
      $key = 'pg:'.md5(get_class($obj)).':'.$pk;

      return $key;

    }

    throw new InvalidObjectForKeyCreationException(
      'PgDataTest only supports pgData objects.',
    );

  }

}
