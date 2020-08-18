<?hh // strict

namespace Zynga\Framework\Service\V2\Request\FileRequest\Exceptions;

use Zynga\Framework\Exception\V1\Exception;

/*
 * Represents that an invalid file info error code was attempted to be
 * imported. See FileInfoErrorEnum for a list of supported values
 */
class FailedToImportInvalidFileInfoErrorException extends Exception {
    <<__Override>>
    public function __construct(int $errorCode) {
        parent::__construct("errorCode=$errorCode");
    }
}