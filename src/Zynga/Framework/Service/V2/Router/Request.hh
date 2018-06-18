<?hh // strict

namespace Zynga\Framework\Service\V2\Router;

use Zynga\Framework\Service\V2\Request\Base as RequestBase;

use Zynga\Framework\Type\V1\StringBox;

class Request extends RequestBase {
  public StringBox $servicePath;

  public function __construct() {
    parent::__construct();
    $this->servicePath = new StringBox();
  }

}
