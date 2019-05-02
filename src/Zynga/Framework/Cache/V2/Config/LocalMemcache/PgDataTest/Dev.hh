<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\LocalMemcache\PgDataTest;

use Zynga\Framework\Cache\V2\Config\LocalMemcache\Base as LocalMemcacheBase;
use Zynga\Framework\PgData\V1\Interfaces\PgRowInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use \Exception;

class Dev extends LocalMemcacheBase {

  public function createKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {

    if ($obj instanceof PgRowInterface) {

      $pk = strval($obj->getPrimaryKeyTyped()->get());
      $key = 'pg:'.md5(get_class($obj).':'.$pk);

      $keyLength = strlen($key);

      error_log('isCached key='.$key.' keyLength='.strlen($key));
      return $key;

    }

    throw new Exception('PgDataTest only supports pgData objects.');

  }

}
