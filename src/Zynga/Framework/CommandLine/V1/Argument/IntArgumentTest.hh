<?hh //strict

namespace Zynga\Framework\CommandLine\V1\Argument;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\CommandLine\V1\Argument\IntArgument;

class IntArgumentTest extends TestCase {
  public function testValidArg(): void {
    $argName = 'testing';
    $argDescription = 'this is a test of the emergency broadcast system';
    $argRequired = true;

    $arg = new IntArgument($argName, $argDescription, $argRequired);

    $this->assertEquals(0, $arg->get());
    $this->assertFalse($arg->hasValue());

    $this->assertTrue($arg->set(12345));
    $this->assertTrue($arg->hasValue());
    $this->assertEquals(12345, $arg->get());

    $this->assertEquals($argDescription, $arg->getDescription());
    $this->assertEquals('--'.$argName.'=\d+', $arg->getArgumentUsage());
  }

  <<
  expectedException(
    "Zynga\Framework\Type\V1\Exceptions\FailedToImportFromFloatException",
  )
  >>
  public function testInvalidArg(): void {
    $argName = 'testing';
    $argDescription = 'this is a test of the emergency broadcast system';
    $argRequired = true;

    $arg = new IntArgument($argName, $argDescription, $argRequired);

    $this->assertEquals(0, $arg->get());
    $arg->set(1234.34);
  }

  <<
  expectedException(
    "Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException",
  )
  >>
  public function testInvalidArgValue(): void {
    $argName = 'testing';
    $argDescription = 'this is a test of the emergency broadcast system';
    $argRequired = true;
    $arg = new IntArgument($argName, $argDescription, $argRequired);

    $argument = '--'.$argName.'=1284.57';
    $arg->parseArgument($argument);

  }

}
