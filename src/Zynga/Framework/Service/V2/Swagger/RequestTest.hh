<?hh //strict

namespace Zynga\Framework\Service\V2\Swagger;

use Zynga\Framework\Service\V2\Swagger\Request as SwaggerRequest;
use Zynga\Framework\StorableObject\V1\Test\Mock\ValidNoRequired;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class RequestTest extends TestCase {

  public function test_construct(): void {
    $obj = new SwaggerRequest();
    $this->assertTrue($obj instanceof SwaggerRequest);
  }

  public function test_parse(): void {
    $obj = new SwaggerRequest();
    $this->assertTrue($obj->parse());
  }

  public function test_consume(): void {
    $storable = new ValidNoRequired();
    $obj = new SwaggerRequest();
    $this->assertTrue($obj->consumeStorableObject($storable));
  }

}
