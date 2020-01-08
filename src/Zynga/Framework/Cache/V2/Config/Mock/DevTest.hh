<?hh //strict

namespace Zynga\Framework\Cache\V2\Config\Mock;

use Zynga\Framework\Cache\V2\Config\Mock\BaseTest;
use Zynga\Framework\Cache\V2\Config\Mock\Dev as ConfigUnderTest;
use Zynga\Framework\Cache\V2\Interfaces\DriverConfigInterface;

class DevTest extends BaseTest {

  public function createConfigUnderTest(): DriverConfigInterface {
    return new ConfigUnderTest();
  }

}
