<?hh //strict

namespace Zynga\Framework\Service\V1\Swagger;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Poker\Framework\Service\Consumer\Swagger\V1\Provider as ConsumerProvider;

class BaseTest extends TestCase {

  public function test_construct(): void {
    $obj = new ConsumerProvider();
    $this->assertTrue($obj instanceof ConsumerProvider);
    $this->assertNotNull($obj->request());
    $this->assertNotNull($obj->response());
    $this->assertTrue($obj->handle());
  }

}
