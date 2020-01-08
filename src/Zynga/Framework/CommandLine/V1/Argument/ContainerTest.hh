<?hh //strict

namespace Zynga\Framework\CommandLine\V1\Argument;

use Zynga\Framework\CommandLine\V1\Argument\Container as ArgumentContainer;
use Zynga\Framework\CommandLine\V1\Argument\StringArgument;
use Zynga\Framework\CommandLine\V1\Argument\IntArgument;
use Zynga\Framework\Environment\SuperGlobals\V1\SuperGlobals;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class ContainerTest extends TestCase {
  private Vector<string> $_argv_copy;
  private int $_argc_copy;

  public function __construct(string $name) {

    parent::__construct($name);

    $this->_argv_copy = Vector {};
    $this->_argc_copy = 0;

  }

  public function setUp(): void {
    $currentCommandLine = SuperGlobals::getScriptCommandLineArguments();
    if ($currentCommandLine !== null && is_array($currentCommandLine)) {
      $this->_argv_copy->addAll($currentCommandLine);
    }
    $this->_argc_copy = $this->_argv_copy->count();
  }

  public function tearDown(): void {
    SuperGlobals::clearScriptCommandLineArguments();
    SuperGlobals::setScriptCommandLineArguments($this->_argv_copy->toArray());
  }

  public function testValid_stringArg(): void {

    $obj = new ArgumentContainer();

    $testArgName = 'monkey';
    $testArgDesc = 'monkeys like to be strings';
    $testArgReq = false;

    $arg = new StringArgument($testArgName, $testArgDesc, $testArgReq);
    $this->assertTrue($obj->addArgument($arg));
    $this->assertEquals(
      StringArgument::class,
      get_class($obj->getArgument($testArgName)),
    );

    // Clear our args and make sure we handle a empty arg stack
    SuperGlobals::clearScriptCommandLineArguments();
    SuperGlobals::setScriptCommandLineArguments(array());
    $this->assertTrue($obj->parse());

    $testArgValue = 'someValueThatSoundsGood';

    $args = array('--'.$testArgName.'='.$testArgValue);
    SuperGlobals::setScriptCommandLineArguments($args);

    $this->assertTrue($obj->parse());
    $this->assertTrue($obj->getArgument($testArgName)->hasValue());
    $this->assertEquals(
      $testArgValue,
      $obj->getArgument($testArgName)->get(),
    );

  }

  public function testValid_intArg(): void {

    $obj = new ArgumentContainer();

    $testArgName = 'monkey';
    $testArgDesc = 'monkeys like to be strings';
    $testArgReq = false;
    $testArgValue = 'someValueThatSoundsGood';

    $testIntArgName = 'emergenc';
    $testIntArgDesc = 'emerg enc int';
    $testIntArgReq = false;
    $testIntValue = 897345785;

    $arg = new StringArgument($testArgName, $testArgDesc, $testArgReq);
    $this->assertTrue($obj->addArgument($arg));

    $intArg =
      new IntArgument($testIntArgName, $testIntArgDesc, $testIntArgReq);
    $this->assertTrue($obj->addArgument($intArg));

    $args = array(
      '--'.$testIntArgName.'='.$testIntValue,
      '--'.$testArgName.'='.$testArgValue,
    );

    SuperGlobals::setScriptCommandLineArguments($args);

    $this->assertTrue($obj->parse());

    $this->assertTrue($obj->getArgument($testArgName)->hasValue());
    $this->assertEquals(
      $testArgValue,
      $obj->getArgument($testArgName)->get(),
    );

    $this->assertTrue($obj->getArgument($testIntArgName)->hasValue());
    $this->assertEquals(
      $testIntValue,
      $obj->getArgument($testIntArgName)->get(),
    );

  }

  <<
  expectedException(
    "Zynga\Framework\CommandLine\V1\Exceptions\UnableToFindArgumentException",
  )
  >>
  public function testInvalidArgument(): void {
    $obj = new ArgumentContainer();
    $obj->getArgument('unknownArg');
  }

  <<
  expectedException(
    "Zynga\Framework\CommandLine\V1\Exceptions\UnableToReadScriptArgumentsException",
  )
  >>
  public function testArgvInvalid(): void {
    SuperGlobals::clearScriptCommandLineArguments();
    $obj = new ArgumentContainer();
    $obj->parse();
  }

  <<
  expectedException(
    "Zynga\Framework\CommandLine\V1\Exceptions\MissingArgumentsException",
  )
  >>
  public function testRequiredArgumentFailure(): void {
    $obj = new ArgumentContainer();

    $testArgName = 'requiredMonkey';
    $testArgDesc = 'monkeys like to be required';
    $testArgReq = true;

    $arg = new StringArgument($testArgName, $testArgDesc, $testArgReq);
    $this->assertTrue($obj->addArgument($arg));
    $this->assertEquals(
      StringArgument::class,
      get_class($obj->getArgument($testArgName)),
    );
    $this->assertTrue($obj->parse());
  }

  <<
  expectedException(
    "Zynga\Framework\CommandLine\V1\Exceptions\MissingArgumentsException",
  )
  >>
  public function testWrongArgType(): void {
    $obj = new ArgumentContainer();

    $testArgName = 'requiredIntMonkey';
    $testArgDesc = 'monkeys like to be required';
    $testArgReq = true;

    $arg = new IntArgument($testArgName, $testArgDesc, $testArgReq);
    $this->assertTrue($obj->addArgument($arg));
    $this->assertEquals(
      IntArgument::class,
      get_class($obj->getArgument($testArgName)),
    );
    $this->assertTrue($obj->parse());
  }

}
