<?hh //strict

namespace Zynga\Framework\CommandLine\V1\Argument;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\CommandLine\V1\Argument\BoolArgument;

class BoolArgumentTest extends TestCase {
  public function testValidArg(): void {
    $argName = 'testing';
    $argDescription = 'this is a test of the emergency broadcast system';
    $argRequired = true;

    $arg = new BoolArgument($argName, $argDescription, $argRequired);

    $this->assertEquals(0, $arg->get());
    $this->assertFalse($arg->hasValue());

    $this->assertTrue($arg->set(true));
    $this->assertTrue($arg->hasValue());
    $this->assertEquals(true, $arg->get());

    $this->assertEquals($argDescription, $arg->getDescription());
    $this->assertEquals(
      '--'.
      $argName.
      '|--with-'.
      $argName.
      '|--no-'.
      $argName.
      '|--without-'.
      $argName,
      $arg->getArgumentUsage(),
    );

    // test the basic --$argName on
    $this->assertTrue($arg->set(false));
    $this->assertTrue($arg->parseArgument('--'.$argName));
    $this->assertTrue($arg->get());

    // test the basic --no-$argName off
    $this->assertTrue($arg->set(true));
    $this->assertTrue($arg->parseArgument('--no-'.$argName));
    $this->assertFalse($arg->get());

    // test the basic --with-$argName on
    $this->assertTrue($arg->set(false));
    $this->assertTrue($arg->parseArgument('--with-'.$argName));
    $this->assertTrue($arg->get());

    // test the basic --without-$argName off
    $this->assertTrue($arg->set(true));
    $this->assertTrue($arg->parseArgument('--without-'.$argName));
    $this->assertFalse($arg->get());

    $this->assertTrue($arg->set(false));
    $this->assertTrue($arg->parseArgument('--another-argument'));
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

    $arg = new BoolArgument($argName, $argDescription, $argRequired);

    $this->assertEquals(0, $arg->get());
    $arg->set(1234.34);
  }

}
