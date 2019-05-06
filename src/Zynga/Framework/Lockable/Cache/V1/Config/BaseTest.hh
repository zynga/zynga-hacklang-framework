<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1\Config;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Lockable\Cache\V1\Config\Mock\Dev;
use Zynga\Framework\Lockable\Cache\V1\Interfaces\LockPayloadInterface;

class BaseTest extends TestCase {

  public function testCreateLockKeyFromStorableKey(): void {
    $obj = new Dev();
    $this->assertEquals(
      'test-value:lock',
      $obj->createLockKeyFromStorableKey('test-value'),
    );
  }

  public function testPayloadObject(): void {
    $obj = new Dev();

    $payload = $obj->getPayloadObject();

    $this->assertInstanceOf(LockPayloadInterface::class, $payload);
    $this->assertGreaterThan(0, $payload->getLockEstablishment());
    $this->assertEquals('TODO', $payload->getBacktrace());

  }

}
