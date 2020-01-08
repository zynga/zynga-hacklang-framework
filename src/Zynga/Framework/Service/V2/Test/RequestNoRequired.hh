<?hh // strict

namespace Zynga\Framework\Service\V2\Test;

use Zynga\Framework\Service\V2\Request\Base;
use Zynga\Framework\Type\V1\UInt64Box;

class RequestNoRequired extends Base {
  public UInt64Box $id;

  public function __construct() {

    $this->id = new UInt64Box();

    parent::__construct();

  }

}
