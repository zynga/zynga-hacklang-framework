<?hh // strict

namespace Zynga\Framework\Service\V2\Request\FileRequest;

use Zynga\Framework\Service\V2\Request\FileRequest\FileInfoError;
use Zynga\Framework\StorableObject\V1\Base as StorableObject;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\UInt32Box;

/*
 * Encapsulation of the $_FILES[fileName] data received in a request
 */
class FileInfo extends StorableObject {

    public StringBox $name;
    public StringBox $type;
    public UInt32Box $size;
    public StringBox $tmp_name;
    public FileInfoError $error;

    <<__Override>>
    public function __construct() {
        parent::__construct();

        $this->name = new StringBox();
        $this->name->setIsRequired(true);
        
        $this->type = new StringBox();
        $this->type->setIsRequired(false);

        $this->size = new UInt32Box();
        $this->size->setIsRequired(true);

        $this->tmp_name = new StringBox();
        $this->tmp_name->setIsRequired(true);

        $this->error = new FileInfoError();
        $this->error->setIsRequired(true);
    }
}
