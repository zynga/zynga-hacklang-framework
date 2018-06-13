<?hh // strict

namespace Zynga\Framework\Service\V1\Test\Authenticated\WithParams;

use Zynga\Framework\Service\V1\Authenticated\Request\ServiceData\Base;
use Zynga\Framework\Type\V1\UInt64Box;

class ServiceData extends Base {
  public UInt64Box $aInt;

  public function __construct() {

    parent::__construct();

    $this->aInt = new UInt64Box();
    $this->aInt->setIsRequired(true);

  }

}
