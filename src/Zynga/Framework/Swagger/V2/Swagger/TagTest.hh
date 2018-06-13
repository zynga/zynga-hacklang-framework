<?hh //strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\Swagger\V2\Swagger\Tag as SwaggerTag;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class TagTest extends TestCase {

  public function testValid(): void {
    $obj = new SwaggerTag();

    $this->assertEquals('', $obj->name->get());
    $this->assertTrue($obj->name->getIsRequired());

    $this->assertEquals('', $obj->description->get());

  }

}
