<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\Mock\NoRequiredFields;

use Zynga\Framework\Cache\V2\Config\Mock\Base as MockBase;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\Type\V1\Interfaces\TypeInterface;

class Base extends MockBase {

  // --
  // For dev|staging|production we allow all the flags to be on to mimic a permissive cache.
  // --
  public function cacheAllowsKeyOverride(): bool {
    return true;
  }

  public function cacheAllowsNonExpiringKeys(): bool {
    return true;
  }

  public function cacheAllowsTTLOverride(): bool {
    return true;
  }

  public function createKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {

    $dataToHash = '';

    foreach ($obj->fields()->getForObject() as $fieldName => $field) {

      if (!$field instanceof TypeInterface) {
        continue;
      }

      if ($dataToHash != '') {
        $dataToHash .= '-';
      }

      $dataToHash .= strval($field->get());

    }

    // now make the generic cache key.
    $key = 'zfc:'.md5(get_class($obj)).':'.md5($dataToHash);

    return $key;
  }

}
