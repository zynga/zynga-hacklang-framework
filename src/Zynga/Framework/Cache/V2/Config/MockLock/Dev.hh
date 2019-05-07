<?hh // strict

namespace Zynga\Framework\Cache\V2\Config\MockLock;

use Zynga\Framework\Cache\V2\Config\LocalMemcache\Base as LocalMemcacheBase;
use Zynga\Framework\Cache\V2\Exceptions\InvalidObjectForKeyCreationException;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired as ValidExampleObject
;

use Zynga\Framework\Exception\V1\Exception;

class Dev extends LocalMemcacheBase {

  public function createKeyFromStorableObject(
    StorableObjectInterface $obj,
  ): string {

    if ($obj instanceof ValidExampleObject) {

      if ($obj->example_uint64->isDefaultValue()[0] !== true) {
        return 'lmc-mock-dev-'.$obj->example_uint64->get().':lock';
      }

      // --
      // JEO: Explicitly simulating how you handle a failure if the passed in value isn't
      // within range or a memcache key of lmc-ve-0 wouldn't be a good use case.
      // --
      throw new InvalidObjectForKeyCreationException(
        'example_unit64 is set to non-valid value='.
        $obj->example_uint64->get(),
      );

    }

    throw new InvalidObjectForKeyCreationException(
      'ValidExampleObject is required obj='.get_class($obj),
    );

  }

}
