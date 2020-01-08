<?hh // strict

namespace Zynga\Framework\Testing\TestCase\V2\Test\Mock;

use Zynga\Framework\Testing\TestCase\V2\Base;

use \ReflectionClass;

class RiggedGetStatus extends Base {

  public function hideLogs(): bool {
    return false;
  }

  public function testOk(): void {
    $this->assertTrue(true);
  }

}
