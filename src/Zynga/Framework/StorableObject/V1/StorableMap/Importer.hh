<?hh // strict

namespace Zynga\Framework\StorableObject\V1\StorableMap;

use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

use Zynga\Framework\StorableObject\V1\Fields;

use Zynga\Framework\StorableObject\V1\Exceptions\ExpectedFieldCountMismatchException;
use Zynga\Framework\StorableObject\V1\Exceptions\MissingKeyFromImportDataException;
use Zynga\Framework\StorableObject\V1\Exceptions\UnsupportedTypeException;
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;

use Zynga\Type\V1\Interfaces\TypeInterface;

use Zynga\Framework\Exception\V1\Exception;

class Importer implements ImportInterface {
  private StorableObjectInterface $_object;

  public function __construct(StorableObjectInterface $object) {
    $this->_object = $object;
  }

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
