<?hh // strict

namespace Zynga\Framework\Swagger\V2\Swagger\DefinitionReference;

use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;

class Import implements ImportInterface {

  public function fromVector(Vector<mixed> $data): bool {
    return false;
  }

  public function fromMap(Map<string, mixed> $data): bool {
    return false;
  }

  public function fromJSON(string $payload): bool {
    return false;
  }

  public function fromBinary(string $payload): bool {
    return false;
  }

}
