<?hh //strict

namespace Zynga\Framework\Testing\TestCase\V2\Config;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Testing\TestCase\V2\Config\Manager as TestManager;

class ManagerTest extends TestCase {

  public function testEnabled(): void {

    $fakeClass = 'not-a-real-test';
    $fakeReason = 'not-a-real-reason';

    // we shouldn't have a test disabled at this point.
    $this->assertFalse(TestManager::enableTest($fakeClass));
    $this->assertTrue(TestManager::isTestEnabled($fakeClass));
    $this->assertEquals('', TestManager::getReason($fakeClass));

    // disable a test
    $this->assertTrue(TestManager::disableTest($fakeClass, $fakeReason));

    // if it's already disabled, refuse to do it again
    $this->assertFalse(TestManager::disableTest($fakeClass, $fakeReason));

    // it should be false, and with a reason
    $this->assertFalse(TestManager::isTestEnabled($fakeClass));
    $this->assertEquals($fakeReason, TestManager::getReason($fakeClass));

    // Restore ourselves to starting state.
    $this->assertTrue(TestManager::enableTest($fakeClass));

    // we shouldn't have a test disabled at this point.
    $this->assertFalse(TestManager::enableTest($fakeClass));
    $this->assertTrue(TestManager::isTestEnabled($fakeClass));
    $this->assertEquals('', TestManager::getReason($fakeClass));

  }

  public function testDisablePattern(): void {
    $fakeClass = 'some-rando-class';
    $fakePattern = '/some\-.*\-class/';
    $fakeReason = 'we-dont-like-your-friend';

    // we shouldn't have a test disabled at this point.
    $this->assertFalse(TestManager::enableTestPattern($fakePattern));
    $this->assertTrue(TestManager::isTestEnabled($fakeClass));
    $this->assertEquals('', TestManager::getReason($fakeClass));

    // disable a test
    $this->assertTrue(
      TestManager::disableTestPattern($fakePattern, $fakeReason),
    );

    // if it's already disabled, refuse to do it again
    $this->assertFalse(
      TestManager::disableTestPattern($fakePattern, $fakeReason),
    );

    // it should be false, and with a reason
    $this->assertFalse(TestManager::isTestEnabled($fakeClass));
    $this->assertEquals($fakeReason, TestManager::getReason($fakeClass));

    // Restore ourselves to starting state.
    $this->assertTrue(TestManager::enableTestPattern($fakePattern));

    // we shouldn't have a test disabled at this point.
    $this->assertFalse(TestManager::enableTestPattern($fakePattern));
    $this->assertTrue(TestManager::isTestEnabled($fakeClass));
    $this->assertEquals('', TestManager::getReason($fakeClass));

  }

}
