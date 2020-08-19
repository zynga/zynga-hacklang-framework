<?hh // strict

namespace Zynga\Framework\Service\V2\Request\FileRequest;

/*
 * Enumeration of all possible error codes in $_FILES[myFile][error]
 * @see https://www.php.net/manual/en/features.file-upload.errors.php
 */
enum FileInfoErrorEnum : int as int {
    Ok = UPLOAD_ERR_OK;
    IniSize = UPLOAD_ERR_INI_SIZE;
    FormSize = UPLOAD_ERR_FORM_SIZE;
    Partial = UPLOAD_ERR_PARTIAL;
    NoFile = UPLOAD_ERR_NO_FILE;
    NoTmpDir = UPLOAD_ERR_NO_TMP_DIR;
    CantWrite = UPLOAD_ERR_CANT_WRITE;
    Extension = UPLOAD_ERR_EXTENSION;
}