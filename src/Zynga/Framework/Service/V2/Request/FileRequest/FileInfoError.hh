<?hh // strict

namespace Zynga\Framework\Service\V2\Request\FileRequest;

use Zynga\Framework\Service\V2\Request\FileRequest\Exceptions\FailedToImportInvalidFileInfoErrorException;
use Zynga\Framework\Service\V2\Request\FileRequest\FileInfoErrorFactory;
use Zynga\Framework\Type\V1\UInt8Box;

/*
 * Encapsulation of the $_FILES[fileName][error] code received in a request containing a file
 */
class FileInfoError extends UInt8Box {

    /**
     * @throws FailedToImportInvalidFileInfoErrorException
     * @throws FailedToImportFromIntegerException
     */
    <<__Override>>
    public function __construct(?int $value = null) {
        parent::__construct();
        if ($value !== null) {
            $this->set($value);
        }
    }

    /**
    * @throws FailedToImportInvalidFileInfoErrorException
    * @throws FailedToImportFromIntegerException
    */
    <<__Override>>
    protected function importFromInteger(int $value): bool {
        if (FileInfoErrorFactory::isValidErrorCode($value) === true) {
            return parent::importFromInteger($value);
        }

        throw new FailedToImportInvalidFileInfoErrorException($value);
    }

    <<__Override>>
    public function __toString(): string {
        $value = $this->get();
        return strval($value);
    }

}
