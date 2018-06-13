<?hh // strict

namespace Zynga\Framework\Service\V1\Test\Authenticated\WithParams;

use Zynga\Framework\Service\V1\Authenticated\Request\Base;
use Zynga\Framework\Service\V1\Authenticated\Request\Interfaces\ServiceDataInterface;
use Zynga\Framework\Service\V1\Test\Authenticated\WithParams\ServiceData;

class Request extends Base {
  public ServiceData $serviceData;

  public function __construct() {
    parent::__construct();
    $this->serviceData = new ServiceData();
  }

  <<__Override>>
  public function serviceData(): ServiceDataInterface {
    return $this->serviceData;
  }

}
