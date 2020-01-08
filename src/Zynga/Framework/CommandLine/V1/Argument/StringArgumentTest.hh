<?hh //strict

namespace Zynga\Framework\CommandLine\V1\Argument;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\CommandLine\V1\Argument\StringArgument;

class StringArgumentTest extends TestCase {
  public function testValidArg(): void {
    $argName = 'testing';
    $argDescription = 'this is a test of the emergency broadcast system';
    $argRequired = true;

    $arg = new StringArgument($argName, $argDescription, $argRequired);

    $this->assertEquals('', $arg->get());
    $this->assertFalse($arg->hasValue());

    $testValue = 'thisIsAValidValue';
    $this->assertTrue($arg->set($testValue));
    $this->assertTrue($arg->hasValue());
    $this->assertEquals($testValue, $arg->get());
    $this->assertTrue($arg->isRequired());

    $this->assertEquals($argDescription, $arg->getDescription());
    $this->assertEquals('--'.$argName.'=.*', $arg->getArgumentUsage());

  }

  public function test_SingleQuoteArg(): void {

    $argName = 'testing';
    $argDescription = 'this is a test of the emergency broadcast system';
    $argRequired = true;

    $arg = new StringArgument($argName, $argDescription, $argRequired);

    $this->assertTrue($arg->parseArgument("--testing='someValue'"));

  }

  public function test_DoubleQuoteArg(): void {

    $argName = 'testing';
    $argDescription = 'this is a test of the emergency broadcast system';
    $argRequired = true;

    $arg = new StringArgument($argName, $argDescription, $argRequired);

    $this->assertTrue($arg->parseArgument('--testing="someValue"'));

  }

  <<
  expectedException(
    "Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException",
  )
  >>
  public function testInvalidArg(): void {
    $argName = 'testing';
    $argDescription = 'this is a test of the emergency broadcast system';
    $argRequired = true;

    $arg = new StringArgument($argName, $argDescription, $argRequired);

    $this->assertEquals('', $arg->get());
    $arg->set('<img');
  }

  <<
  expectedException(
    "Zynga\Framework\Type\V1\Exceptions\FailedToImportFromStringException",
  )
  >>
  public function testInvalidArg_BadArgValue(): void {
    $argName = 'testing';
    $argDescription = 'this is a test of the emergency broadcast system';
    $argRequired = true;

    $arg = new StringArgument($argName, $argDescription, $argRequired);

    $this->assertEquals('', $arg->get());
    $arg->parseArgument('--testing=<img');
  }

}
