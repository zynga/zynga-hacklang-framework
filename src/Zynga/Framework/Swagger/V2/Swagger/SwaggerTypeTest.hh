<?hh //strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Factory\V2\Test\MockState as FactoryMockState;
use Zynga\Framework\StorableObject\V1\Test\Mock\Broken\ValidButHasConstructorArgs;
use Zynga\Framework\StorableObject\V1\Test\Mock\Broken\ValidNestedVectorButBrokenChild;
use Zynga\Framework\StorableObject\V1\Test\Mock\ValidNestedVector;
use Zynga\Framework\Swagger\V2\Swagger;
use Zynga\Framework\Swagger\V2\Swagger\SwaggerType;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\BoolBox;
use Zynga\Framework\Type\V1\EmailBox;
use Zynga\Framework\Type\V1\FixedLengthStringBox;
use Zynga\Framework\Type\V1\FloatBox;
use Zynga\Framework\Type\V1\HttpResponseCodeBox;
use Zynga\Framework\Type\V1\Int32Box;
use Zynga\Framework\Type\V1\Int64Box;
use Zynga\Framework\Type\V1\StringBox;
use Zynga\Framework\Type\V1\UInt16Box;
use Zynga\Framework\Type\V1\UInt32Box;
use Zynga\Framework\Type\V1\UInt64Box;
use Zynga\Framework\Type\V1\UInt8Box;
use Zynga\Framework\Type\V1\UrlBox;
use Zynga\Framework\Dynamic\V1\Exceptions\MissingRequiredParametersException;
use Zynga\Framework\Swagger\V2\Exceptions\UnknownZyngaTypeException;

use Zynga\Poker\Type\Client\Id\V1\Box as ClientIdBox;
use Zynga\Poker\Type\Client\Version\V1\Box as ClientVersionBox;
use Zynga\Poker\Type\Game\Id\V1\Box as GameIdBox;
use Zynga\Poker\Type\Snid\V1\Box as SnidBox;
use Zynga\Poker\Type\Uid\V1\Box as UidBox;

class SwaggerTypeTest extends TestCase {

  public function testSwaggerObjReference(): void {
    $swag = new Swagger();
    $obj = new SwaggerType($swag);
    $this->assertTrue($obj->swagger() instanceof Swagger);
  }

  public function testTypeConversion(): void {
    $swag = new Swagger();
    $obj = new SwaggerType($swag);

    // Integer types.
    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(UInt64Box::class));
    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(SnidBox::class));
    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(UidBox::class));
    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(ClientIdBox::class));
    $this->assertTrue(
      $obj->convertZyngaTypeToSwaggerType(HttpResponseCodeBox::class),
    );
    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(Int64Box::class));
    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(UInt32Box::class));
    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(Int32Box::class));
    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(UInt16Box::class));
    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(UInt8Box::class));

    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(BoolBox::class));

    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(StringBox::class));
    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(UrlBox::class));

    $this->assertTrue(
      $obj->convertZyngaTypeToSwaggerType('StorableVector<'.StringBox::class.'>'),
    );
    $this->assertTrue(
      $obj->convertZyngaTypeToSwaggerType('StorableMap<'.StringBox::class.'>'),
    );

    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(FixedLengthStringBox::class));
    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(EmailBox::class));
    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(ClientIdBox::class));
    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(GameIdBox::class));
    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(BoolBox::class));
    $this->assertTrue($obj->convertZyngaTypeToSwaggerType(ClientVersionBox::class));
  }

  public function testUnknownType(): void {
    $swag = new Swagger();
    $obj = new SwaggerType($swag);
    $this->expectException(UnknownZyngaTypeException::class);
    $obj->convertZyngaTypeToSwaggerType('unknownOrInvalidType');
  }

  public function testUnknownType_storableMap(): void {
    $swag = new Swagger();
    $obj = new SwaggerType($swag);
    $this->expectException(UnknownZyngaTypeException::class);
    $obj->convertZyngaTypeToSwaggerType('StorableMap<Int>');
  }

  public function testUnknownType_storableVector(): void {
    $swag = new Swagger();
    $obj = new SwaggerType($swag);
    $this->expectException(UnknownZyngaTypeException::class);
    $obj->convertZyngaTypeToSwaggerType('StorableVector<String>');
  }

  public function testUnknownType_TestingArrayForIssues(): void {
    $swag = new Swagger();
    $obj = new SwaggerType($swag);
    $obj->type->set(SwaggerType::ARRAY_TYPE);
    $this->expectException(UnknownZyngaTypeException::class);
    $obj->convertZyngaTypeToSwaggerType('StorableVector');
  }

  public function testUnknownType_TestingArrayTypeCOnversionForIssues(): void {
    $swag = new Swagger();
    $obj = new SwaggerType($swag);
    $obj->type->set(SwaggerType::ARRAY_TYPE);
    $this->expectException(UnknownZyngaTypeException::class);
    $obj->convertZyngaTypeToSwaggerType('StorableVector<MagicBox>');
  }

  public function testNestedStorableVector(): void {
    $swag = new Swagger();
    $obj = new SwaggerType($swag);
    $this->assertTrue(
      $obj->convertZyngaTypeToSwaggerType(ValidNestedVector::class),
    );
  }

  public function testNestedStorableVector_BrokeObjectPath(): void {

    // Discovered this bug, and taht we were covering it already via testing the nested storable vector
    $swag = new Swagger();
    $obj = new SwaggerType($swag);
    $this->expectException(UnknownZyngaTypeException::class);
    $obj->convertZyngaTypeToSwaggerType(
      'Zynga\StorableObect\V1\Test\Mock\ValidNestedVector'
    );

  }

  public function testBrokenConstructor(): void {
    $swag = new Swagger();
    $obj = new SwaggerType($swag);
    $this->expectException(MissingRequiredParametersException::class);
    $obj->convertZyngaTypeToSwaggerType(ValidButHasConstructorArgs::class);
  }

  public function testNestedStorableVector_BrokeChildObject(): void {
    $swag = new Swagger();
    $obj = new SwaggerType($swag);
    $this->expectException(MissingRequiredParametersException::class);
    $obj->convertZyngaTypeToSwaggerType(ValidNestedVectorButBrokenChild::class);
  }

  public function testAddSubTypeToSchema_InvalidObject(): void {

    // Discovered this bug, and taht we were covering it already via testing the nested storable vector
    $swag = new Swagger();
    $obj = new SwaggerType($swag);
    $this->assertFalse($obj->addSubTypeToSchema('Zynga\Framework\Exception\V1\Exception'));

  }
}
