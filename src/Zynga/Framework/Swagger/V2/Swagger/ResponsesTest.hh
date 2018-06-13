<?hh //strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\Swagger\V2\Swagger;
use Zynga\Framework\Swagger\V2\Swagger\Response as SwaggerResponse;
use Zynga\Framework\Swagger\V2\Swagger\Responses;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class ResponsesTest extends TestCase {

  public function testValid(): void {

    $swag = new Swagger();

    $obj = new Responses();

    // our stack should be empty at this moment.
    list($isDefaultValue, $defaultFields) = $obj->isDefaultValue();
    $this->assertTrue($isDefaultValue);
    $this->assertEquals(0, $defaultFields->count());
    $this->assertEquals(0, $obj->getMap()->count());

    // add a success response
    $resp = new SwaggerResponse($swag);
    $resp->description->set('someSuccessDescription');
    $this->assertTrue($obj->addSuccessResponse($resp));

    list($isDefaultValue, $defaultFields) = $obj->isDefaultValue();
    $this->assertFalse($isDefaultValue);
    $this->assertEquals(0, $defaultFields->count());
    $this->assertEquals(1, $obj->getMap()->count());

    // see what the exports look like
    $this->assertEquals(
      '{"200": {"description":"someSuccessDescription","schema":{"type":"object"}}}',
      $obj->export()->asJSON(),
    );
    $this->assertEquals('', $obj->export()->asBinary());
    $this->assertEquals(0, $obj->export()->asMap()->count());

    // add a failure response
    $resp = new SwaggerResponse($swag);
    $resp->description->set('someFailureDescription');
    $this->assertTrue($obj->addFailureResponse($resp));

    list($isDefaultValue, $defaultFields) = $obj->isDefaultValue();
    $this->assertFalse($isDefaultValue);
    $this->assertEquals(0, $defaultFields->count());
    $this->assertEquals(2, $obj->getMap()->count());

    // see what the exports look like
    $this->assertEquals(
      '{"200": {"description":"someSuccessDescription","schema":{"type":"object"}},"400": {"description":"someFailureDescription","schema":{"type":"object"}}}',
      $obj->export()->asJSON(),
    );
    $this->assertEquals('', $obj->export()->asBinary());
    $this->assertEquals(0, $obj->export()->asMap()->count());

    $this->assertEquals(
      0,
      $obj->fields()->getFieldsAndTypesForObject()->count(),
    );
    $this->assertEquals(0, $obj->fields()->getForObject()->count());
    $this->assertEquals(
      0,
      $obj->fields()->getRequiredFieldsWithDefaultValues()->count(),
    );

    $vec = Vector {};
    $this->assertFalse($obj->import()->fromVector($vec));

    $this->assertFalse($obj->import()->fromBinary(''));

    $this->assertFalse($obj->import()->fromJSON(''));

    $map = Map {};
    $this->assertFalse($obj->import()->fromMap($map));

    $this->assertTrue($obj->setIsRequired(false));
    $this->assertTrue($obj->getIsRequired());

    $this->assertTrue($obj->setIsDefaultValue(true));

    list($isDefaultValue, $defautFields) = $obj->isDefaultValue();
    $this->assertTrue($isDefaultValue);

    $this->assertTrue($obj->setIsDefaultValue(false));

    list($isDefaultValue, $defautFields) = $obj->isDefaultValue();
    $this->assertFalse($isDefaultValue);

  }

  public function test_addSuccessResponse(): void {
    // add a success response, should exception since description isn't set.
    $swag = new Swagger();
    $obj = new Responses();
    $resp = new SwaggerResponse($swag);
    $this->assertFalse($obj->addSuccessResponse($resp));
  }

  public function test_addFailureResponse(): void {
    // add a success response, should exception since description isn't set.
    $swag = new Swagger();
    $obj = new Responses();
    $resp = new SwaggerResponse($swag);
    $this->assertFalse($obj->addFailureResponse($resp));
  }


  public function test_unimplemented(): void {
    $obj = new Responses();
    $this->expectException(Exception::class);
    $obj->fields()->getTypedField('somefield');
  }

}
