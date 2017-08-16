<?hh //strict

namespace Zynga\Framework\Testing\MockState\V2;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCaseBase;

class MockStateTest extends TestCaseBase {

  public function test_validUsage(): void {

    // enableMocks
    $this->assertTrue(MockState::enableMocks());

  }

}
