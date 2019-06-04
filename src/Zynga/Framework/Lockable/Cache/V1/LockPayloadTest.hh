<?hh // strict

namespace Zynga\Framework\Lockable\Cache\V1;

use Zynga\Framework\Lockable\Cache\V1\LockPayload;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class LockPayloadTest extends TestCase {

  public function testIsLockStillValid_Success(): void {
    $obj = new LockPayload();
    $obj->setLockEstablishment(time());
    $this->assertTrue($obj->isLockStillValid(300));
  }

  public function testIsLockStillValid_Failure(): void {
    $obj = new LockPayload();
    $obj->setLockEstablishment(0);
    $this->assertFalse($obj->isLockStillValid(300));
  }

}
