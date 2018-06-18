<?hh // strict

namespace Zynga\Framework\Service\V2\Authenticated\Request;

use Zynga\Framework\Service\V2\Interfaces\Authenticated\RequestInterface;
use Zynga\Framework\Service\V2\Request\Base as RequestBase;

/**
 * Base authenticated request class. This is the starting point for all
 * authenticated services
 */
abstract class Base extends RequestBase implements RequestInterface {

    /**
     * PHP magic function
     */
    public function __construct() {
        parent::__construct();

    }

}
