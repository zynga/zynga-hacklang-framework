<?hh //strict

namespace Zynga\Framework\Datadog\V2\Config\Poker;

use Zynga\Framework\Datadog\V2\Config\BaseTest;
use Zynga\Framework\Environment\DevelopmentMode\V1\DevelopmentMode;

class DevTest extends BaseTest {
  public function getDriverName(): string {
    return 'Poker';
  }
  public function getDevelopmentMode(): int {
    return DevelopmentMode::DEV;
  }
}
