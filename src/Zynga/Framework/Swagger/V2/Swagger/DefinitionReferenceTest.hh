<?hh //strict

namespace Zynga\Framework\Swagger\V2\Swagger;

use Zynga\Framework\Swagger\V2\Swagger\DefinitionReference;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\BoolBox;

class DefinitionReferenceTest extends TestCase {
  public function test_isRequired(): void {
    $obj = new DefinitionReference();
    $this->assertFalse($obj->getIsRequired());
    $this->assertTrue($obj->setIsRequired(false));
  }

  public function test_isDefaultValue(): void {
    $obj = new DefinitionReference();

    list($isDefaultValue, $defaultValues) = $obj->isDefaultValue();
    $this->assertTrue($isDefaultValue);
  }

  public function testSetDefaultValue(): void {
    $obj = new DefinitionReference();

    $this->assertTrue($obj->setIsDefaultValue(true));

    $this->assertTrue($obj->setDefaultValue('some-value'));
    $this->assertFalse($obj->setDefaultValue(7890234));
    $this->assertTrue(is_string($obj->get()));

    $this->assertTrue($obj->set('some-other-value'));
    $this->assertFalse($obj->set(3875));

    list($isDefaultValue, $defaultValues) = $obj->isDefaultValue();
    $this->assertFalse($isDefaultValue);

    $this->assertEquals('some-other-value', $obj->get());
  }

  public function test_fieldDiscovery(): void {
    $obj = new DefinitionReference();

    // exercise the field discoverer, default state with no object
    $this->assertEquals(0, $obj->fields()->getForObject()->count());
    $this->assertEquals(
      0,
      $obj->fields()->getFieldsAndTypesForObject()->count(),
    );
    $this->assertEquals(
      0,
      $obj->fields()->getRequiredFieldsWithDefaultValues('')->count(),
    );
  }

  public function test_importingFromVector(): void {
    $obj = new DefinitionReference();

    // exercise the importer
    $vec = Vector {};
    $this->assertFalse($obj->import()->fromVector($vec));
  }

  public function test_importingFromMap(): void {
    $obj = new DefinitionReference();

    $map = Map {};
    $this->assertFalse($obj->import()->fromMap($map));
    $this->assertFalse($obj->import()->fromJSON(''));
    $this->assertFalse($obj->import()->fromBinary(''));
  }

  public function test_exporter(): void {
    $obj = new DefinitionReference();

    $obj->set('some-other-value');

    // exercise the exporter
    $this->assertEquals(
      '"$ref":"#/definitions/some-other-value"',
      $obj->export()->asJSON(),
    );
    $this->assertEquals(0, $obj->export()->asMap()->count());
    $this->assertEquals('', $obj->export()->asBinary());
  }

  public function test_exporterAsJson_emptyObject(): void {
    $obj = new DefinitionReference();
    $this->assertEquals('', $obj->export()->asJSON(''));
  }

  public function test_exporterAsJson_emptyClassName(): void {
    $obj = new DefinitionReference();
    $obj->setIsDefaultValue(false);
    $this->assertEquals('', $obj->export()->asJSON(''));
  }

  public function testReset(): void {
    $definitionReference = new DefinitionReference();
    $this->assertTrue($definitionReference->reset());
  }

  public function test_unimplemented(): void {
    $obj = new DefinitionReference();
    $this->expectException(Exception::class);
    $obj->fields()->getTypedField('somefield');
  }

  public function testValuesNotEqual(): void {
    $true = new DefinitionReference();
    $true->set("true");
    $false = new DefinitionReference();
    $false->set("false");
    $this->assertFalse($true->equals($false));
  }

  public function testNullNotEqual(): void {
    $true = new DefinitionReference();
    $true->set("true");
    $this->assertFalse($true->equals(null));
  }

  public function testWrongClassNotEqual(): void {
    $true = new DefinitionReference();
    $true->set("true");
    $bool = new BoolBox();
    $bool->set(true);
    $this->assertFalse($true->equals($bool));
  }

  public function testValuesEqual(): void {
    $true = new DefinitionReference();
    $true->set("true");
    $true2 = new DefinitionReference();
    $true2->set("true");
    $this->assertTrue($true->equals($true2));
  }

}
