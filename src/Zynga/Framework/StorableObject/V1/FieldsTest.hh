<?hh //strict

namespace Zynga\Framework\StorableObject\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\StorableObject\V1\Test\Mock\Valid;
use Zynga\Framework\StorableObject\V1\Test\Mock\Nofields;
use Zynga\Framework\StorableObject\V1\Test\Mock\ValidNested;
use
  Zynga\Framework\StorableObject\V1\Test\Mock\Broken\ValidButBrokenFieldsGetForObject
;
use Zynga\Framework\Type\V1\FloatBox;

class FieldsTest extends TestCase {

  public function test_getTypedField(): void {
    $obj = new Valid();
    $field = $obj->fields()->getTypedField('example_float');
    $this->assertEquals(
      'Zynga\Framework\Type\V1\FloatBox',
      get_class($field),
    );
  }

  public function test_getRequiredFieldsWithDefaultValues_NoFields(): void {
    $obj = new Nofields();
    $data = $obj->fields()->getRequiredFieldsWithDefaultValues('none');
    $this->assertEquals(0, $data->count());
  }

  public function test_getRequiredFieldsWithDefaultValues_ContextTraversal(
  ): void {

    $obj = new ValidNested();
    $obj->otherVar->setIsRequired(true);

    $defaults = $obj->fields()->getRequiredFieldsWithDefaultValues();
    $this->assertEquals(1, $defaults->count());

    $obj->validObj->setIsRequired(true);

    $defaults = $obj->fields()->getRequiredFieldsWithDefaultValues();
    $this->assertEquals(3, $defaults->count());

  }

  <<expectedException("Zynga\Framework\Exception\V1\Exception")>>
  public function test_brokenFieldFetch(): void {
    $obj = new ValidButBrokenFieldsGetForObject();
    $obj->fields()->getRequiredFieldsWithDefaultValues('no-real-context');
  }

}
