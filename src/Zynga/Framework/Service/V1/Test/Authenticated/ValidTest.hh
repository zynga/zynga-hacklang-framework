<?hh //strict

namespace Zynga\Framework\Service\V1\Test\Authenticated;

use Zynga\Framework\Service\V1\Test\Authenticated\Valid;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class ValidTest extends TestCase {
  public function testAuthenticatedHandle(): void {
    $valid = new Valid();
    $this->assertTrue($valid->authenticatedHandle());
  }
}
