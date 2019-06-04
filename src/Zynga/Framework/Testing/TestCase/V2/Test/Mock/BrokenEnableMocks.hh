<?hh // strict

namespace Zynga\Framework\Testing\TestCase\V2\Test\Mock;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Testing\TestCase\V2\Base;

class BrokenEnableMocks extends Base {
  public function doEnableMocks(): void {
    throw new Exception('We logged but failed');
  }
}
