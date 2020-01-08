<?hh // strict

namespace Zynga\Framework\Type\V1\Test\Ranged;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\Test\Ranged\RangedValueGeneratorMock;
use Zynga\Framework\Type\V1\Test\ValidValue;

use Zynga\PHPUnit\V2\Exceptions\AssertionFailedException;

class TestCaseTest extends TestCase {

  public function test_OOB_Min_Failure(): void {
    $mock = new RangedValueGeneratorMock('testOOBMin');
    $this->expectException(AssertionFailedException::class);
    $mock->testOOBMin();
  }

  public function test_OOB_Max_Failure(): void {
    $mock = new RangedValueGeneratorMock('testOOBMax');
    $this->expectException(AssertionFailedException::class);
    $mock->testOOBMax();
  }

  public function test_Mockery(): void {
    $mock = new RangedValueGeneratorMock('testMockery');
    $this->assertFalse($mock->hideLogs());
    $mock->setUp();
    $mock->setGenerateValidValues(Vector {});
    $values = $mock->generateValidValues();
    $this->assertEquals(0, count($values));
    $mock->setGenerateInvalidValues(Vector {});
    $values = $mock->generateInvalidValues();
    $this->assertEquals(0, count($values));
    $this->assertTrue(true);
  }

}
