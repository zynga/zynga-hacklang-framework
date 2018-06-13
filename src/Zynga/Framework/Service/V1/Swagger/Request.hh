<?hh // strict

namespace Zynga\Framework\Service\V1\Swagger;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\Service\V1\Interfaces\RequestInterface;

use Zynga\Environment\V1\SuperGlobals;
use Zynga\Framework\Exception\V1\Exception;

class Request extends StorableObject implements RequestInterface {

  public function parse(): bool {
    return true;
  }

  public function consumeStorableObject(StorableObjectInterface $obj): bool {
    return true;
  }

}
