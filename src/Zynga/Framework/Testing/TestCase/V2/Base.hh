<?hh // strict

namespace Zynga\Framework\Testing\TestCase\V2;

use Zynga\Framework\Environment\CodePath\V1\CodePath;
use Zynga\Framework\Performance\V1\Tracker as PerformanceTracker;
use Zynga\Framework\Environment\DevelopmentMode\V1\DevelopmentMode;
use Zynga\Framework\Testing\TestCase\V2\Config\Manager as TestManager;
use Zynga\Framework\Testing\MockState\V2\MockState;
use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Performance\V1\XHProfiler as PerformanceProfiler;
use Zynga\PHPUnit\V2\TestCase;
use Zynga\PHPUnit\V2\Environment\OutputBuffering;

// --
// include legacy casino platform to get the old xhprofiler brought in.
// TODO: port the legacy xhprofiler.
// --
// require_once CodePath::getRoot().'/shared/casino-platform/include/path.php';
// use \zyCasino_App_Performance_XHProfiler;

// --
// JEO: Bring in the phpunit interfaces.
// --
// public function run(PHPUnit_Framework_TestResult $result = null);
// public function toString();

use \PHPUnit_Framework_Test;
use \PHPUnit_Framework_SelfDescribing;
use \PHPUnit_Framework_Assert;
use \PHPUnit_Framework_TestResult;
use \PHPUnit_Framework_TestCase;

use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Dynamic\V1\DynamicMethodCall;
use Zynga\PHPUnit\V2\Exceptions\AssertionFailedException;
use \ReflectionClass;

abstract class Base extends TestCase
  implements PHPUnit_Framework_SelfDescribing {

  private static bool $_hasBeforeClassMethod = false;

  private ?PerformanceTracker $_perf;
  private bool $_hideLogs = true;

  public function hideLogs(): bool {
    return $this->_hideLogs;
  }

  public function setHideLogs(bool $hide): bool {
    $this->_hideLogs = $hide;
    return true;
  }

  public function doHideLogs(): void {

    // --
    // Zynga specific log trap, where we turn off our old logger
    //
    // Feel free to overload this functionality with whatever you need.
    // --

    try {
      DynamicMethodCall::callMethod(
        'Zynga\Legacy\Log',
        'setHideAllLogs',
        Vector {true},
      );
    } catch (Exception $e) {
      // The legacy logger is not present, and thats okay.
    }

  }

  /**
   * We do not support the static method version of setUpBeforeClass, we
   * modified phpunit to offer doSetupBeforeClass as a method on your test.
   */
  final public function setUpBeforeClass(): void {}

  public function doDevelopmentModeReset(): void {
    DevelopmentMode::reset();
  }

  public function doEnableMocks(): void {
    MockState::enableMocks();
  }

  public function doDatabaseFactoryResultSetReset(): void {
    DatabaseFactory::resetResultSets();
  }

  public function resetTestingEnvironment(): void {

    try {
      $this->doDevelopmentModeReset();
    } catch (Exception $e) {
      // Noop - do nothing.
    }

    try {
      $this->doEnableMocks();
    } catch (Exception $e) {
      // Noop - do nothing.
    }

    try {
      $this->doDatabaseFactoryResultSetReset();
    } catch (Exception $e) {
      // Noop - do nothing.
    }

    try {
      $this->doHideLogs();
    } catch (Exception $e) {
      // noop - do nothing,
    }
  }

  /**
   * A hookable function that runs before any of your tests run.
   *
   * Note: This behaves the same way phpunit does and is executed before the
   * test is ran, not before __construct() is called.
   *
   * @return bool doSetupBeforeClass success state
   */
  public function doSetUpBeforeClass(): bool {

    $this->resetTestingEnvironment();

    return true;

  }

  /**
   * A hookable function that is ran after any of your test ran.
   *
   * Note: This behaves the same way phpunit does and is executed after the
   * tests are ran, not after __destruct() is called.
   *
   * @return bool doTearDownAfterClass success state
   */
  public function doTearDownAfterClass(): bool {

    $this->resetTestingEnvironment();

    return true;

  }

  /**
   *
   * Our default setup function handles the enabling / disabling of tests for
   *  our platform. This means you need to call me if you want to be a good
   *  citizen.
   *
   */
  public function setUp(): void {

    if ($this->hideLogs() === true) {
      // when hideLogs is enabled allow all output to happen.
      $this->expectOutputRegex('/.*/');
      $this->doHideLogs();
    }

    if ($this->isEnabled() !== true) {
      $context = $this->getTestCaseContext();
      $testReason = TestManager::getReason($context);
      $this->markTestSkipped($testReason);
    }

  }

  public function tearDown(): void {
    if ($this->hideLogs() === true) {
      DynamicMethodCall::callMethod(
        'Zynga\Legacy\V1\Log',
        'setHideAllLogs',
        Vector {false},
        true,
      );
    }
  }

  public function getTestCaseContext(): string {
    $context = get_class($this);
    return $context;
  }

  public function isEnabled(): bool {
    $context = $this->getTestCaseContext();
    return TestManager::isTestEnabled($context);
  }

  public function assertionFailureExpected(): void {
    $this->expectException(AssertionFailedException::class);
  }

  public function assertClassImplements(string $expected, mixed $value): void {

    $isObject = is_object($value);

    $this->assertTrue($isObject, 'value is not an object');

    $implements = class_implements($value);

    $isClassImplementsArray = is_array($implements);

    $this->assertTrue(
      $isClassImplementsArray,
      'class_implements returned non array for value',
    );

    $isExpectedInArray = in_array($expected, $implements);

    $this->assertTrue(
      $isExpectedInArray,
      'expected='.
      $expected.
      ' is not within implementaiton list='.
      implode(',', $implements),
    );

  }

  public function perf(): PerformanceTracker {
    if ($this->_perf === null) {
      $this->_perf = new PerformanceTracker();
    }

    return $this->_perf;
  }

  public function assertPerf(string $timerName, int $timeout): void {
    $delta = $this->perf()->getTimer($timerName)->getElapsedTime();
    $this->assertLessThan($timeout, $delta);
  }

  public function getProfileDirectory(): string {
    return CodePath::getRoot().'/tmp/xh-profiles';

  }

  public function startProfiling(): void {
    PerformanceProfiler::setProfileDir($this->getProfileDirectory());
    PerformanceProfiler::startProfiling();
  }

  public function stopProfiling(): void {
    $backtrace = debug_backtrace(DEBUG_BACKTRACE_IGNORE_ARGS, 2);

    $frame = $backtrace[1];

    $name = 'UNKNOWN';
    if (array_key_exists('class', $frame)) {
      $name = $frame['class'];
    }

    if (array_key_exists('function', $frame)) {
      $name .= '_'.$frame['function'];
    }

    PerformanceProfiler::stopProfiling($name);

  }

  public function expectOutput(string $expected): bool {
    return $this->expectOutputString($expected);
  }

  public function expectOutputPregMatch(string $pattern): bool {
    return $this->expectOutputRegex($pattern);
  }

  /**
   * Basic no operation test, allows you quickly stand up a test for your class. (JEO: This has no coverage stats)
   * @return bool test success
   */
  public function testNoop(): bool {
    // PHPUNIT emit a warning as it thinks this class is a test suite.
    $this->assertTrue(true);
    return true;
  }

  public function testMemoryLimit(): bool {
    $memory_limit = ini_get('memory_limit');
    $this->assertEquals(9223372036854775807, $memory_limit);
    return true;
  }

}
