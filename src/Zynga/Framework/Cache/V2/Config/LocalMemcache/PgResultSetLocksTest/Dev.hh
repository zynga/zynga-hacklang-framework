<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\LocalMemcache\PgResultSetLocksTest;

use Zynga\Framework\Cache\V2\Config\LocalMemcache\Base as LocalMemcacheBase;
use Zynga\Framework\Cache\V2\Exceptions\InvalidObjectForKeyCreationException;
use Zynga\Framework\PgData\V1\Interfaces\PgResultSetInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use \Exception;

class Dev extends LocalMemcacheBase {

  public function createKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {

    if ($obj instanceof PgResultSetInterface) {

      $checksum = $obj->createChecksum();

      $key = 'pgrs:'.$checksum.':lock';

      return $key;

    }

    throw new InvalidObjectForKeyCreationException(
      'PgDataTest only supports pgResultSetInterface objects.',
    );

  }

  public function getTTL(): int {
    return 30;
  }

}
