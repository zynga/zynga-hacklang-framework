<?hh // strict

namespace Zynga\Framework\Service\V1\Interfaces;

use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

interface RequestInterface extends StorableObjectInterface {
  public function parse(): bool;
  public function consumeStorableObject(StorableObjectInterface $obj): bool;
}
