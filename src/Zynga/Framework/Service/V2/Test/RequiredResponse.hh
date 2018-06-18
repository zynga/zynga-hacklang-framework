<?hh // strict

namespace Zynga\Framework\Service\V2\Test;

use Zynga\Framework\Service\V2\Response\Base;

use Zynga\Framework\Type\V1\UInt64Box;

class RequiredResponse extends Base {
  public UInt64Box $id;
  public UInt64Box $yetAnotherValue;

  public function __construct() {

    $this->id = new UInt64Box();
    $this->id->setIsRequired(true);

    $this->yetAnotherValue = new UInt64Box();
    $this->yetAnotherValue->setIsRequired(true);

    parent::__construct();

  }
}
