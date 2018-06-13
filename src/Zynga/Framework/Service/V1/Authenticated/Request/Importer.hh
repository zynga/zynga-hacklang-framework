<?hh // strict

namespace Zynga\Framework\Service\V1\Authenticated\Request;

use \InvalidArgumentException;
use Zynga\Framework\Service\V1\Authenticated\Request\Base as ServiceRequest;
use Zynga\Framework\Service\V1\Authenticated\Request\Exceptions\IncorrectChecksumException;
use Zynga\Framework\StorableObject\V1\Object\Importer as BaseImporter;

class Importer extends BaseImporter {
    <<__Override>>
    public function fromMap(Map<string, mixed> $data): bool {

      // parent throws exceptions upon failure
      parent::fromMap($data);

      if ($this->_object instanceof ServiceRequest) {

        $serviceRequest = $this->_object;
        $expected = $serviceRequest->reconstructChecksum();
        $given = $serviceRequest->checksum();

        if ($given->get() === $expected->get()) {
            return true;
        }

        throw new IncorrectChecksumException("Expected=".$expected.", given=".$given);
      }

      throw new InvalidArgumentException("_object was not of type ServiceRequest");

    }
}
