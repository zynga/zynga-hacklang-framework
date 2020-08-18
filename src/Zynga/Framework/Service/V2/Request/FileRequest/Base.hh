<?hh // partial

namespace Zynga\Framework\Service\V2\Request\FileRequest;

use Zynga\Framework\Service\V2\Request\Base as BaseRequest;
use Zynga\Framework\Service\V2\Request\FileRequest\FileInfo;
use Zynga\Framework\Type\V1\StringBox;

/*
 * Service request supporting and requiring a file as part of
 * the request
 */
abstract class Base extends BaseRequest {

    public FileInfo $fileInfo;

    <<__Override>>
    public function __construct() {
        parent::__construct();
        $this->fileInfo = new FileInfo();
    }

    /*
     * The supplied name of the file expected to be in the request, i.e.
     * $_FILES[myFileName]
     */
    abstract public function getExpectedFileName(): StringBox;

}
