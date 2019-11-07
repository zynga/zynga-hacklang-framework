<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\Mock\InMemory\PgResultSet;

use Zynga\Framework\Cache\V2\Config\Mock\InMemory\PgDataBase;
use Zynga\Framework\Cache\V2\Exceptions\InvalidObjectForKeyCreationException;
use Zynga\Framework\PgData\V1\PgCachedResultSet;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use \Exception;

class Dev extends PgDataBase {

  public function createKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {

    if ($obj instanceof PgCachedResultSet) {

      $checksum = $obj->createChecksum();

      $key = 'pgrs:'.$checksum;

      return $key;

    }

    throw new InvalidObjectForKeyCreationException(
      'PgDataTest only supports PgCachedResultSet objects.',
    );

  }

  public function cacheAllowsTTLOverride(): bool {
    return true;
  }
}
