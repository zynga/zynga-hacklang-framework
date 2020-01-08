<?hh // strict

namespace Zynga\Framework\Testing\TestCase\V2\Test\Mock;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Testing\TestCase\V2\Base;

class BrokenDoHideLogs extends Base {
  public function doHideLogs(): void {
    throw new Exception('We logged but failed');
  }
}
