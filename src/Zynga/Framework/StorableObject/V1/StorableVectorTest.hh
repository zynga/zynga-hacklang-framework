<?hh //strict

namespace Zynga\Framework\StorableObject\V1;

use Zynga\Framework\StorableObject\V1\Interfaces\ExportInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\FieldsInterface;
use Zynga\Framework\StorableObject\V1\Interfaces\ImportInterface;
use Zynga\Framework\StorableObject\V1\StorableVector;
use Zynga\Framework\StorableObject\V1\Test\Mock\Nofields as NofieldsStorableObject;
use Zynga\Framework\StorableObject\V1\Test\Mock\Valid as ValidStorableObject;
use Zynga\Framework\StorableObject\V1\Test\Mock\ValidNestedVector;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\UInt64Box;

class StorableVectorTest extends TestCase {
        public function test_valid(): void {
        $randomValue = mt_rand(1, 1000000);

        $testKey = 0;

        $testValue = new UInt64Box();
        $testValue->set($randomValue);

        $vec = new StorableVector();

        // verify the count and that we are at a default value
        $this->assertEquals(0, $vec->count());
        list($isDefaultValue, $defaultValues) = $vec->isDefaultValue();
        $this->assertTrue($isDefaultValue);

        // add a item to the vector
        $this->assertTrue($vec->add($testValue));
        $this->assertTrue($vec->set($testKey, $testValue));

        // see if the item actually stored to the map
        $testObject = $vec->get($testKey);

        $this->assertEquals(UInt64Box::class, get_class($testObject));

        if ($testObject instanceof UInt64Box) {
            $this->assertEquals($randomValue, $testObject->get());
        }

        // verify the count should be 1
        $this->assertEquals(1, $vec->count());
        $this->assertEquals(1, $vec->getVector()->count());
    }

    public function test_invalid_key(): void {
        // make sure we return null when asking for something that isn't there.
        $vec = new StorableVector();
        $someOtherKey = 3;
        $this->assertEquals(null, $vec->get($someOtherKey));
    }

    public function test_isDefaultValueFlag(): void {
        $vec = new StorableVector();
        $this->assertTrue($vec->setIsDefaultValue(false));

        list($isDefaultValue, $defaultValues) = $vec->isDefaultValue();
        $this->assertFalse($isDefaultValue);

        $this->assertTrue($vec->setIsDefaultValue(true));
        list($isDefaultValue, $defaultValues) = $vec->isDefaultValue();
        $this->assertTrue($isDefaultValue);
    }

    public function test_isRequiredFlag(): void {
        $vec = new StorableVector();
        $this->assertFalse($vec->getIsRequired());
        $this->assertTrue($vec->setIsRequired(true));
        $this->assertTrue($vec->getIsRequired());
        $this->assertTrue($vec->setIsRequired(false));
        $this->assertFalse($vec->getIsRequired());
    }

    public function test_std_interface(): void {
        $map = new StorableVector();
        $this->assertTrue($map->fields() instanceof FieldsInterface);
        $this->assertTrue($map->import() instanceof ImportInterface);
        $this->assertTrue($map->export() instanceof ExportInterface);
    }

    public function test_isDefaultValue(): void {
        // Test the isDefaultValue detection.
        $newVec = new StorableVector();

        // we should see it matching defaults as there isn't anything in the vec
        list($isDefaultValue, $defaultFields) = $newVec->isDefaultValue();
        $this->assertTrue($isDefaultValue);

        $anotherTestValue = new UInt64Box();
        $newVec->add($anotherTestValue);

        // we should see it still matching defaults, because the child object being defaults.
        list($isDefaultValue, $defaultFields) = $newVec->isDefaultValue();
        $this->assertTrue($isDefaultValue);

        // force a value into the another test value
        $anotherTestValue->set(mt_rand());

        // should not be set to defaults now
        list($isDefaultValue, $defaultFields) = $newVec->isDefaultValue();
        $this->assertFalse($isDefaultValue);

        $this->assertTrue($newVec->setIsDefaultValue(true));

        list($isDefaultValue, $defaultFields) = $newVec->isDefaultValue();
        $this->assertTrue($isDefaultValue);

        $this->assertTrue($newVec->setIsDefaultValue(false));

        list($isDefaultValue, $defaultFields) = $newVec->isDefaultValue();
        $this->assertFalse($isDefaultValue);
    }

    public function testReset(): void {
        $vector = new StorableVector();
        $this->assertTrue($vector->reset());
    }
}
