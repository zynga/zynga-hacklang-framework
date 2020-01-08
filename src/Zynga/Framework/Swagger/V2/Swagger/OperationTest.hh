<?hh //strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Swagger\V2\Swagger\Operation;

class OperationTest extends TestCase {
  public function testValid(): void {
    $obj = new Operation();
    $this->assertEquals(0, $obj->tags->count());
    $this->assertEquals('', $obj->summary->get());
    $this->assertEquals('', $obj->description->get());
    $this->assertTrue($obj->description->getIsRequired());
    $this->assertEquals('', $obj->operationId->get());
    $this->assertEquals(0, $obj->parameters->count());
    $this->assertEquals(0, $obj->responses->getMap()->count());
    $this->assertTrue($obj->responses->getIsRequired());
    $this->assertFalse($obj->deprecated->get());
  }
}
