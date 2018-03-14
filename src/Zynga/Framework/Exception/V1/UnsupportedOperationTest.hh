<? // strict

namespace Zynga\Framework\Exception\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Exception\V1\UnsupportedOperation as UnsupportedOperationException;

class UnsupportedOperationTest extends TestCase {

  public function testThrowingUnsupportedOperationException(): void {
    $this->expectException(UnsupportedOperationException::class);
    throw new UnsupportedOperationException();
  }

}
