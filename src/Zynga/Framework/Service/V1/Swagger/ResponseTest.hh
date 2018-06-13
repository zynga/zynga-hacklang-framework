<?hh //strict

namespace Zynga\Framework\Service\V1\Swagger;

use Zynga\Framework\Service\V1\Swagger\Response as SwaggerResponse;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class ResponseTest extends TestCase {

  public function test_construct(): void {
    $obj = new SwaggerResponse();
    $this->assertTrue($obj instanceof SwaggerResponse);
    $this->assertFalse($obj->success()->get());
    $this->assertEquals(0, $obj->code()->get());
    $this->assertEquals(0, $obj->message()->count());
    $this->assertTrue($obj->addMessage('some-mess'));
  }

}
