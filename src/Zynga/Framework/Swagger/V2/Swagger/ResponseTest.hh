<?hh //strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Swagger\V2\Swagger;
use Zynga\Framework\Swagger\V2\Swagger\Response;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class ResponseTest extends TestCase {

  public function testValid(): void {
    $swag = new Swagger();
    $obj = new Response($swag);
    $this->assertEquals('', $obj->description->get());
    $this->assertTrue($obj->description->getIsRequired());
    $this->assertTrue($obj->swagger() instanceof Swagger);
  }

}
