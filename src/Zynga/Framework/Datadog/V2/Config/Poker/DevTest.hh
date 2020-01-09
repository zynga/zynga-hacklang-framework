<?hh //strict

namespace Zynga\Framework\Datadog\V2\Config\Poker;

use Zynga\Framework\Datadog\V2\Config\BaseTest;
use Zynga\Framework\Datadog\V2\Interfaces\DriverConfigInterface;

class DevTest extends BaseTest {

  public function getDriverConfigToTest(): DriverConfigInterface {
    return new Dev();
  }
}
