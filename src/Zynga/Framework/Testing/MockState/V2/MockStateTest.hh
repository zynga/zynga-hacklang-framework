<?hh //strict

namespace Zynga\Framework\Testing\MockState\V2;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCaseBase;
use Zynga\Framework\Testing\MockState\V2\MockState;
use Zynga\Framework\Testing\MockState\V2\Mocks\FactoryExample;
use Zynga\Framework\Testing\MockState\V2\Mocks\SimpleClassExample;

class MockStateTest extends TestCaseBase {

  public function test_validUsage(): void {

    $this->assertTrue(MockState::disableMocks());
    $this->assertTrue(MockState::enableMocks());
    $this->assertTrue(MockState::enableMocks());

  }

  public function test_addMockableClass_validClasses(): void {

    // make sure our entrance conditions are pure.
    $this->assertTrue(FactoryExample::clear());
    $this->assertTrue(SimpleClassExample::clear());
    $this->assertFalse(FactoryExample::getUseMockDrivers());
    $this->assertFalse(SimpleClassExample::getMockState());


    $this->assertTrue(MockState::addMockableClass('Zynga\Framework\Testing\MockState\V2\Mocks\FactoryExample'));
    $this->assertTrue(MockState::addMockableClass('Zynga\Framework\Testing\MockState\V2\Mocks\SimpleClassExample'));

    // cleanup the mockables before we start this test.
    $this->assertTrue(MockState::clearMockables());

    // Nothing should change
    $this->assertTrue(MockState::disableMocks());
    $this->assertFalse(FactoryExample::getUseMockDrivers());
    $this->assertFalse(SimpleClassExample::getMockState());

    // Now flop the puppy on
    $this->assertTrue(MockState::enableMocks());
    $this->assertTrue(FactoryExample::getUseMockDrivers());
    $this->assertTrue(SimpleClassExample::getMockState());

    // cleanup on the way out
    $this->assertTrue(MockState::clearMockables());
    $this->assertTrue(FactoryExample::clear());
    $this->assertTrue(SimpleClassExample::clear());
    
  }

}
