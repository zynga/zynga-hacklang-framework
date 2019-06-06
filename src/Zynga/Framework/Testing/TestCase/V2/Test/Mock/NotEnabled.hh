<?hh // strict

namespace Zynga\Framework\Testing\TestCase\V2\Test\Mock;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Testing\TestCase\V2\Base;

class NotEnabled extends Base {
  public function isEnabled(): bool {
    return false;
  }
  // We need to turn off our output buffering for this test.
  public function hideLogs(): bool {
    return false;
  }
}
