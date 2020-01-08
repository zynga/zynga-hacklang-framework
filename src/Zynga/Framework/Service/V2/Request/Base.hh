<?hh // strict

namespace Zynga\Framework\Service\V2\Request;

use Zynga\Framework\StorableObject\V1\Base as StorableObject;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;

use Zynga\Framework\Service\V2\Interfaces\RequestInterface;

use Zynga\Framework\Environment\SuperGlobals\V1\SuperGlobals;
use Zynga\Framework\Exception\V1\Exception;

abstract class Base extends StorableObject implements RequestInterface {

  public function parse(): bool {
    try {
      $data = SuperGlobals::getRequestAsMap();
      $this->import()->fromMap($data);
      return true;
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function consumeStorableObject(StorableObjectInterface $obj): bool {
    try {
      $this->import()->fromMap($obj->export()->asMap());
      return true;
    } catch (NoFieldsFoundException $e) {
      // we allow the users to configure with no request fields.
      return true;
    } catch (Exception $e) {
      throw $e;
    }
  }

}
