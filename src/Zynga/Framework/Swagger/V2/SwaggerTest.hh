<?hh //strict

namespace Zynga\Framework\Swagger\V2;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Swagger\V2\Swagger;

class SwaggerTest extends TestCase {

  public function testValid(): void {
    $obj = new Swagger();
    $this->assertTrue($obj->swagger->getIsRequired());
    $this->assertEquals(Swagger::SWAGGER_VERSION, $obj->swagger->get());
    $this->assertTrue($obj->info->getIsRequired());
    $this->assertFalse($obj->host->getIsRequired());
    $this->assertFalse($obj->basePath->getIsRequired());
    $this->assertTrue($obj->paths->getIsRequired());
    $this->assertFalse($obj->definitions->getIsRequired());
  }

}
