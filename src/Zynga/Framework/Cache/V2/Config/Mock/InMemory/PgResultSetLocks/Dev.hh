<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\Mock\InMemory\PgResultSetLocks;

use Zynga\Framework\Cache\V2\Config\InMemory\Base as InMemoryBase;
use Zynga\Framework\Cache\V2\Exceptions\InvalidObjectForKeyCreationException;
use Zynga\Framework\PgData\V1\PgCachedResultSet;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use \Exception;

class Dev extends InMemoryBase {

  public function createKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {

    if ($obj instanceof PgCachedResultSet) {

      $checksum = $obj->createChecksum();

      $key = 'pgrs:'.$checksum.':lock';

      return $key;

    }

    throw new InvalidObjectForKeyCreationException(
      'PgDataTest only supports PgCachedResultSet objects.',
    );

  }

  public function getTTL(): int {
    return 30;
  }

}
