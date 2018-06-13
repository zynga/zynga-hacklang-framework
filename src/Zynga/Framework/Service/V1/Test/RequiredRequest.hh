<?hh // strict

namespace Zynga\Framework\Service\V1\Test;

use Zynga\Framework\Service\V1\Request\Base;
use Zynga\Framework\Type\V1\UInt64Box;

class RequiredRequest extends Base {
  public UInt64Box $id;
  public UInt64Box $someOtherValue;

  public function __construct() {

    $this->id = new UInt64Box();
    $this->id->setIsRequired(true);

    $this->someOtherValue = new UInt64Box();
    $this->someOtherValue->setIsRequired(true);

    parent::__construct();

  }

}
