<?hh // strict

namespace Zynga\Framework\Service\V1\Test\Router\Services\Mock\V1;

use Zynga\Framework\Service\V1\Response\Base as ResponseBase;
use Zynga\Framework\Type\V1\StringBox;

class Response extends ResponseBase {
  public StringBox $request_class;

  public function __construct() {

    parent::__construct();

    $this->request_class = new StringBox();

  }

}
