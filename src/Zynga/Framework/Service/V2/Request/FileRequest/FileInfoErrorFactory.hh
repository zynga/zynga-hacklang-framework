<?hh // strict

namespace Zynga\Framework\Service\V2\Request\FileRequest;

use Zynga\Framework\Service\V2\Request\FileRequest\FileInfoError;
use Zynga\Framework\Service\V2\Request\FileRequest\FileInfoErrorEnum;

/*
 * Factory for creating/validating a FileInfoError
 */
class FileInfoErrorFactory {

    public static function isValidErrorCode(int $errorCode): bool {
        return FileInfoErrorEnum::coerce($errorCode) !== null;
    }

    public static function ok(): FileInfoError {
        return new FileInfoError(FileInfoErrorEnum::Ok);
    }

    public static function iniSize(): FileInfoError {
        return new FileInfoError(FileInfoErrorEnum::IniSize);
    }

    public static function formSize(): FileInfoError {
        return new FileInfoError(FileInfoErrorEnum::FormSize);
    }

    public static function partial(): FileInfoError {
        return new FileInfoError(FileInfoErrorEnum::Partial);
    }

    public static function noFile(): FileInfoError {
        return new FileInfoError(FileInfoErrorEnum::NoFile);
    }

    public static function noTmpDir(): FileInfoError {
        return new FileInfoError(FileInfoErrorEnum::NoTmpDir);
    }

    public static function cantWrite(): FileInfoError {
        return new FileInfoError(FileInfoErrorEnum::CantWrite);
    }

    public static function extension(): FileInfoError {
        return new FileInfoError(FileInfoErrorEnum::Extension);
    }

}
