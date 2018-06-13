<?hh //strict

namespace Zynga\Framework\Service\V1\Request;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

use Zynga\Framework\Environment\SuperGlobals\V1\SuperGlobals;
use Zynga\Framework\Service\V1\Test\Request;

use Zynga\Framework\StorableObject\V1\Test\Mock\Valid;
use Zynga\Framework\StorableObject\V1\Test\Mock\Nofields;
use Zynga\Framework\StorableObject\V1\Test\Mock\Broken\ValidButBrokenExporter;

class BaseTest extends TestCase {

  public function testValidRequest(): void {

    // Test basic object working.
    $testKey = 'id';
    $testValue = mt_rand();

    $obj = new Request();
    $this->assertTrue($obj->id->set($testValue));
    $this->assertEquals($testValue, $obj->id->get());

    $testValue = mt_rand();

    SuperGlobals::setRequestPairing($testKey, $testValue);

    $obj->parse();

    $this->assertEquals($testValue, $obj->id->get());

  }

  /**
   * @expectedException Zynga\Framework\Type\V1\Exceptions\FailedToImportFromFloatException
   */
  public function testInvalidRequest_FloatToInt(): void {
    // Test basic object working.
    $testKey = 'id';
    $testValue = 3.1459;

    SuperGlobals::setRequestPairing($testKey, $testValue);

    $obj = new Request();
    $obj->parse();

  }

  /**
   * @expectedException Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException
   */
  public function testInvalidRequest_FloatStringToInt(): void {
    // Test basic object working.
    $testKey = 'id';
    $testValue = '3.1459';

    SuperGlobals::setRequestPairing($testKey, $testValue);

    $obj = new Request();
    $obj->parse();

  }

  public function testConsumeStorable_ValidObject(): void {
    $data = new Valid();
    $obj = new Request();
    $this->assertTrue($obj->consumeStorableObject($data));
  }

  public function testConsumeStorable_Nofields(): void {
    $data = new Nofields();
    $obj = new Request();
    $this->assertTrue($obj->consumeStorableObject($data));
  }

  /**
   * @expectedException Zynga\Framework\Exception\V1\Exception
   */
  public function testConsumeStorable_ExceptionTrap(): void {
    $data = new ValidButBrokenExporter();
    $obj = new Request();
    $obj->consumeStorableObject($data);
  }

}
