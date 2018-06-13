<?hh //strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\Swagger\V2\Swagger;
use Zynga\Framework\Swagger\V2\Swagger\Schema;

use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidStorableObject;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class SchemaTest extends TestCase {

  public function testValidSchema(): void {
    $swag = new Swagger();
    $obj = new Schema($swag);
    $this->assertEquals('object', $obj->type->get());
    $this->assertEquals(0, $obj->properties->count());
    $this->assertEquals(0, $obj->required->count());
    $this->assertEquals(
      'Zynga_Framework_Swagger_V2_Swagger_Schema',
      $obj->getReferenceNameForStorable($obj),
    );
  }

  public function test_conversion(): void {
    $swag = new Swagger();
    $obj = new Schema($swag);

    $v_obj = new ValidStorableObject();

    $obj->convertStorableToSchema($v_obj);

    $this->assertEquals('object', $obj->type->get());
    $this->assertEquals(3, $obj->properties->count());
    $this->assertEquals(2, $obj->required->count());

  }

}
