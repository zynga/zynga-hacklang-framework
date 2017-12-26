<?hh //strict

namespace Zynga\Framework\CommandLine\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\CommandLine\V1\Test\Mock\Valid;
use Zynga\Framework\CommandLine\V1\Test\Mock\TossedException;
use Zynga\Framework\CommandLine\V1\Test\Mock\HasRequiredArguments;
use Zynga\Framework\CommandLine\V1\Argument\Container as ArgumentContainer;

use Zynga\Framework\Performance\V1\Interfaces\PerformanceInterface;
use Zynga\Framework\Environment\SuperGlobals\V1\SuperGlobals;

class BaseTest extends TestCase {
  private array<string> $_argv;

  public function __construct(string $name) {
    parent::__construct($name);
    $this->_argv = array();
  }

  public function setUp(): void {
    parent::setUp();
    $currentArgv = SuperGlobals::getScriptCommandLineArguments();
    if (is_array($currentArgv)) {
      $this->_argv = $currentArgv;
    }
  }

  public function tearDown(): void {
    parent::tearDown();
    SuperGlobals::setScriptCommandLineArguments($this->_argv);
  }

  public function testErrorMessage(): void {
    $obj = new Valid();
    $ts = $obj->getTime();
    $val = 'time='.$ts;
    $obj->error($val);
    $this->expectOutput(date(Base::DATE_FORMAT, $ts).' [ERROR] - '.$val."\n");
  }

  public function testInfoMessage(): void {
    $obj = new Valid();
    $ts = $obj->getTime();
    $val = 'time='.time();
    $obj->info($val);
    $this->expectOutput(date(Base::DATE_FORMAT, $ts).' [INFO] - '.$val."\n");
  }

  public function testMessage(): void {
    $obj = new Valid();
    $ts = $obj->getTime();
    $val = 'time='.time();
    $obj->message($val);
    $this->expectOutput(date(Base::DATE_FORMAT, $ts).' '.$val."\n");
  }

  public function testQuietMode(): void {
    $obj = new Valid();
    $this->assertInstanceOf(ArgumentContainer::class, $obj->args());
    $this->assertInstanceOf(PerformanceInterface::class, $obj->perf());
    $this->assertFalse($obj->getQuietMode());
    $this->assertTrue($obj->enableQuietMode());
    $this->assertTrue($obj->getQuietMode());
    $this->assertTrue($obj->message('TEST_QUIET'));
    $this->assertEquals(0, $obj->run());
  }

  public function testUsage(): void {
    $testArgs = array();
    $testArgs[] = '--reqAndHasValue=someValue';
    SuperGlobals::setScriptCommandLineArguments($testArgs);
    $obj = new HasRequiredArguments();
    $this->assertEquals(0, $obj->handleRun());
    $this->assertEquals(236563200, $obj->getTime());
    $this->assertEquals(255, $obj->run());
  }

  public function testHandleExceptionTrap(): void {
    $testArgs = array();
    $obj = new TossedException();
    //$obj->handleRun();
    $this->assertEquals(255, $obj->run());
  }
}
