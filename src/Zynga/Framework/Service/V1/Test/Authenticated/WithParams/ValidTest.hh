<?hh //strict

namespace Zynga\Framework\Service\V1\Test\Authenticated\WithParams;

use Zynga\Framework\Service\V1\Test\Authenticated\WithParams\Valid;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class ValidTest extends TestCase {
  public function testAuthenticatedHandle(): void {
    $valid = new Valid();
    $this->assertTrue($valid->authenticatedHandle());
    $this->assertInstanceOf(
      \Zynga\Framework\Service\V1\Test\Authenticated\WithParams\Request::class,
      $valid->request()
    );
    $this->assertInstanceOf(
      \Zynga\Framework\Service\V1\Test\Authenticated\WithParams\Response::class,
      $valid->response()
    );
  }
}
