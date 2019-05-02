<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\InMemory\Mock;

use Zynga\Framework\Cache\V2\Config\InMemory\Base as InMemoryBase;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidExampleObject
;

use Zynga\Framework\Exception\V1\Exception;

class Production extends InMemoryBase {

  public function createKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {

    if ($obj instanceof ValidExampleObject) {

      if ($obj->example_uint64->isDefaultValue()[0] !== true) {
        return 'lmc-ve-'.$obj->example_uint64->get();
      }

      throw new Exception(
        'example_unit64 is set to non-valid value='.
        $obj->example_uint64->get(),
      );

    }

    throw new Exception(
      'ValidExampleObject is required obj='.get_class($obj),
    );

  }

}
