<?hh //strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Swagger\V2\Swagger\Path as SwaggerPath;

class PathTest extends TestCase {

  public function testValid(): void {
    $obj = new SwaggerPath();
    $this->assertEquals(0, $obj->get->parameters->count());
    $this->assertEquals(0, $obj->post->parameters->count());
  }

}
