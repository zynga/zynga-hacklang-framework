<?hh //strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Swagger\V2\Swagger\Parameter as SwaggerParameter;
use Zynga\Framework\Swagger\V2\Swagger;

class ParameterTest extends TestCase {

  public function testValid(): void {

    $swag = new Swagger();

    $obj = new SwaggerParameter($swag);

    $this->assertEquals('', $obj->name->get());
    $this->assertTrue($obj->name->getIsRequired());

    $this->assertEquals('', $obj->in->get());
    $this->assertTrue($obj->in->getIsRequired());

    $this->assertEquals('', $obj->description->get());

    $this->assertFalse($obj->required->get());
    $this->assertTrue($obj->required->getIsRequired());

  }

}
