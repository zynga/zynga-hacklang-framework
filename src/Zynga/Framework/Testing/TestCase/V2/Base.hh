<?hh // strict

namespace Zynga\Framework\Testing\TestCase\V2;

use Zynga\Framework\Environment\CodePath\V1\CodePath;
use Zynga\Framework\Performance\V1\Tracker as PerformanceTracker;
use Zynga\Framework\Environment\TemporaryDirectory\V1\TemporaryDirectory;
use Zynga\Framework\Environment\DevelopmentMode\V1\DevelopmentMode;
use Zynga\Framework\Testing\TestCase\V2\Config\Manager as TestManager;
use Zynga\Framework\Testing\MockState\V2\MockState;

use Zynga\Framework\Database\V2\Factory as DatabaseFactory;
use Zynga\Framework\Database\V2\Interfaces\DriverInterface as DatabaseDriverInterface;
use Zynga\Framework\Database\V2\Driver\Mock as MockDatabaseDriver;

use Zynga\Framework\Database\V2\UserSharded\Factory as UserShardedDatabaseFactory;
use Zynga\Framework\Database\V2\UserSharded\Interfaces\DriverInterface as UserShardedDatabaseDriverInterface;
use Zynga\Framework\Database\V2\UserSharded\Driver\Mock as UserShardedMockDatabaseDriver;

use Zynga\Framework\Testing\TestCase\V2\TestCaseShim;

// --
// include legacy casino platform to get the old xhprofiler brought in.
// TODO: port the legacy xhprofiler.
// --
require_once CodePath::getRoot().'/shared/casino-platform/include/path.php';

use \zyCasino_App_Performance_XHProfiler;
use \PHPUnit_Framework_TestCase;
use \PHPUnit_Framework_AssertionFailedError;

// --
// JEO: Bring in the phpunit interfaces.
// --
// public function run(PHPUnit_Framework_TestResult $result = null);
// public function toString();
use \PHPUnit_Framework_Test;
use \PHPUnit_Framework_SelfDescribing;
use \PHPUnit_Framework_Assert;
use \PHPUnit_Framework_TestResult;
use Zynga\Exception;
use Zynga\Legacy\V1\Log as LegacyLogger;

abstract class Base implements PHPUnit_Framework_Test,  PHPUnit_Framework_SelfDescribing {

  private static bool $_hasBeforeClassMethod = false;
  private ?PerformanceTracker $_perf;
  private TestCaseShim $_testCaseShim;

  /**
   * Constructs a test case with the given name.
   *
   * @param string $name
   * @param array  $data
   * @param string $dataName
   */
  public function __construct(string $name) {
    // @codeCoverageIgnoreStart
    $this->_testCaseShim = new TestCaseShim($name);
    $this->_testCaseShim->setTest($this);
    // @codeCoverageIgnoreEnd
  }

  public function hideLogs(): bool {
    return true;
  }

  /*
  public function __construct(string $name = null, mixed $data = [], string $dataName = '' ) {

    parent::__construct($name, $data, $dataName);

  }
  */

  /**
   * We do not support the static method version of setUpBeforeClass, we
   * modified phpunit to offer doSetupBeforeClass as a method on your test.
   */
  final public static function setUpBeforeClass(): void {
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

    // error_log('JEO doSetUpBeforeClass=RAN');
    DevelopmentMode::reset();
    MockState::enableMocks();

    return true;

  }

  /**
   * We do not support the static method version of tearDownAfterClass, we
   * modified phpunit to offer doSetupBeforeClass as a method on your test.
   */
  final public static function tearDownAfterClass(): void {
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
    // error_log('doTearDownAfterClass=RAN');

    DevelopmentMode::reset();
    MockState::enableMocks();
    return true;
  }

  /**
   *
   * Our default setup function handles the enabling / disabling of tests for
   *  our platform. This means you need to call me if you want to be a good
   *  citizen.
   *
   * JEO: This really should be the only spot we see codeCoverageIgnore. However
   *  given enough time to make a meta mock here we could get around this.
   *
   * @codeCoverageIgnore
   */
  public function setUp(): void {
    if ($this->hideLogs() === true) {
      ob_start();
      LegacyLogger::setHideAllLogs(true);
    }

    if ($this->isEnabled() !== true) {
      $context = $this->getTestCaseContext();
      $testReason = TestManager::getReason($context);
      $this->markTestSkipped($testReason);
    }
  }

  public function tearDown(): void {
    if ($this->hideLogs() === true) {
      ob_end_clean();
      LegacyLogger::setHideAllLogs(false);
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

  public function assertionFailureExpected(): void {
    $this->expectException(PHPUnit_Framework_AssertionFailedError::class);
  }

  public function assertClassImplements(string $expected, mixed $value): void {

    $isObject = is_object($value);

    $this->assertTrue($isObject, 'value is not an object');

    $implements = class_implements($value);

    $isClassImplementsArray = is_array($implements);

    $this->assertTrue($isClassImplementsArray, 'class_implements returned non array for value');

    $isExpectedInArray = in_array($expected, $implements);

    $this->assertTrue($isExpectedInArray, 'expected=' . $expected . ' is not within implementaiton list=' . implode(',', $implements));

  }

  public function dumpIncludeChain(): void {
    $incFiles = get_included_files();
    $incFiles = var_export($incFiles, true);
    file_put_contents(
      TemporaryDirectory::get().'/included-files.php',
      "<?php\n \$fileStack = ".$incFiles.";",
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

  public function startProfiling(): void {
    // TODO: xhprofiler needs to be brought up into the fold.
    // zyCasino_App_Performance_XHProfiler::startProfiling();
  }

  public function stopProfiling(string $name): void {
    // TODO: xhprofiler needs to be brought up into the fold.
    // zyCasino_App_Performance_XHProfiler::stopProfiling($name);
  }

  protected function expectOutput(string $expected): void {
    $this->assertEquals($expected, ob_get_contents());
  }

  protected function expectOutputPregMatch(
    string $pattern,
    string $subject,
  ): void {
    $this->assertEquals(1, preg_match($pattern, $subject));
  }

  public function loadDatabaseResultsForTest(string $function): bool {
    $dbh = DatabaseFactory::factory(DatabaseDriverInterface::class,'Mock');
    if ( $dbh instanceof MockDatabaseDriver ) {
      $this->assertTrue($dbh->loadResultsForTest(static::class, $function));
    }
    return true;
  }

  public function loadUserShardedDatabaseResultsForTest(string $function): bool {
    $dbh = UserShardedDatabaseFactory::factory(UserShardedDatabaseDriverInterface::class,'Mock');
    if ( $dbh instanceof UserShardedMockDatabaseDriver ) {
      $this->assertTrue($dbh->loadResultsForTest(static::class, $function . '_UserSharded'));
    }
    return true;
  }

  // --
  // JEO: begin adapter functions to expose phpunit to the hack strict world.
  // --
  // All of these are currently not covered due to the impact of phpunits reliance
  // upon global singletons. We would have to undo that to make all of these testable.
  // --
  // @codeCoverageIgnoreStart
  /**
   * Mark the test as skipped.
   *
   * @param string $message
   *
   * @throws PHPUnit_Framework_SkippedTestError
   *
   * @since Method available since Release 3.0.0
   */
  public function markTestSkipped(string $message = ''): bool {
    $this->_testCaseShim->markTestSkipped($message);
    return true;
  }

/**
 * Asserts that a condition is true.
 *
 * @param bool   $condition
 * @param string $message
 *
 * @throws PHPUnit_Framework_AssertionFailedError
 */
 public function assertTrue(bool $condition, string $message = ''): void {
   $this->_testCaseShim->assertTrue($condition, $message);
 }

 /**
 * Asserts that two variables are equal.
 *
 * @param mixed  $expected
 * @param mixed  $actual
 * @param string $message
 * @param float  $delta
 * @param int    $maxDepth
 * @param bool   $canonicalize
 * @param bool   $ignoreCase
 */
 public function assertEquals(mixed $expected, mixed $actual, string $message = '', float $delta = 0.0, int $maxDepth = 10, bool $canonicalize = false, bool $ignoreCase = false): void {
   $this->_testCaseShim->assertEquals($expected, $actual, $message, $delta, $maxDepth, $canonicalize, $ignoreCase);
 }

 /**
  * @param mixed $exception
  *
  * @since Method available since Release 5.2.0
  */
 public function expectException<T>(?classname<T> $exception): void {
   $this->_testCaseShim->expectException($exception);
 }

 /**
 * Asserts that a value is smaller than another value.
 *
 * @param mixed  $expected
 * @param mixed  $actual
 * @param string $message
 *
 * @since Method available since Release 3.1.0
 */
 public function assertLessThan(mixed $expected, mixed $actual, string $message = ''): void {
   $this->_testCaseShim->assertLessThan($expected, $actual, $message);
 }

 /**
 * Returns a string representation of the test case.
 *
 * @return string
 */
 public function toString(): string {
   return $this->_testCaseShim->toString();
 }

 /**
 * Runs the test case and collects the results in a TestResult object.
 * If no TestResult object is passed a new one will be created.
 *
 * @param PHPUnit_Framework_TestResult $result
 *
 * @return PHPUnit_Framework_TestResult
 *
 * @throws PHPUnit_Framework_Exception
 */
 public function run(?PHPUnit_Framework_TestResult $result = null): PHPUnit_Framework_TestResult {

   $runResult = $this->_testCaseShim->run($result);

   if ( $runResult instanceof PHPUnit_Framework_TestResult ) {
     return $runResult;
   }

   /*
   if ( $result instanceof PHPUnit_Framework_TestResult ) {
     return $result;
   }
   */

   throw new Exception('run failure both runResult and result were null');

 }

 /**
 * Counts the number of test cases executed by run(TestResult result).
 *
 * @return int
 */
 public function count(): int {
   return $this->_testCaseShim->count();
 }

 /**
  * Asserts that a string starts with a given prefix.
  *
  * @param string $prefix
  * @param string $string
  * @param string $message
  *
  * @since Method available since Release 3.4.0
  */
 public function assertStringStartsWith(string $prefix, string $string, string $message = ''): void {
   $this->_testCaseShim->assertStringStartsWith($prefix, $string, $message);
 }

 /**
 * Asserts that a variable is of a given type.
 *
 * @param string $expected
 * @param mixed  $actual
 * @param string $message
 *
 * @since Method available since Release 3.5.0
 */
  public function assertInstanceOf<T>(classname<T> $expected, mixed $actual, string $message = ''): void {
    $this->_testCaseShim->assertInstanceOf($expected, $actual, $message);
  }

  /**
 * Asserts that a condition is false.
 *
 * @param bool   $condition
 * @param string $message
 *
 * @throws PHPUnit_Framework_AssertionFailedError
 */
 public function assertFalse(bool $condition, string $message = ''): void {
   $this->_testCaseShim->assertFalse($condition, $message);
 }

 /**
 * Asserts that a variable is not null.
 *
 * @param mixed  $actual
 * @param string $message
 */
  public function assertNotNull(mixed $actual, string $message = ''): void {
    $this->_testCaseShim->assertNotNull($actual, $message);
  }

 /**
  * Asserts that a variable is not empty.
 *
 * @param mixed  $actual
 * @param string $message
 *
 * @throws PHPUnit_Framework_AssertionFailedError
 */
 public function assertNotEmpty(mixed $actual, string $message = ''): void {
   $this->_testCaseShim->assertNotEmpty($actual, $message);
 }

 /**
 * Asserts that a value is greater than another value.
 *
 * @param mixed  $expected
 * @param mixed  $actual
 * @param string $message
 *
 * @since Method available since Release 3.1.0
 */
 public function assertGreaterThan(mixed $expected, mixed $actual, string $message = ''): void {
   $this->_testCaseShim->assertGreaterThan($expected, $actual, $message);
 }

 /**
 * Asserts that a variable is empty.
 *
 * @param mixed  $actual
 * @param string $message
 *
 * @throws PHPUnit_Framework_AssertionFailedError
 */
 public function assertEmpty(mixed $actual, string $message = ''): void {
   $this->_testCaseShim->assertEmpty($actual, $message);
 }

 /**
 * Fails a test with the given message.
 *
 * @param string $message
 *
 * @throws PHPUnit_Framework_AssertionFailedError
 */
 public function fail(string $message = ''): void {
   $this->_testCaseShim->fail($message);
 }

 /**
  * Asserts that a condition is not false.
  *
  * JEO: The documentation for this in phpunit said bool, however if you swap
  * this to the strict type of bool within hacklang you will see errors where
  * intent is to test for everything but the explicit false value.
  *
  * @param mixed   $condition
  * @param string $message
  *
  * @throws PHPUnit_Framework_AssertionFailedError
  */
 public function assertNotFalse(mixed $condition, string $message = ''): void {
   $this->_testCaseShim->assertNotFalse($condition, $message);
 }

 /**
  * Asserts that a variable is null.
  *
  * @param mixed  $actual
  * @param string $message
  */
 public function assertNull(mixed $actual, string $message = ''): void {
   $this->_testCaseShim->assertNull($actual, $message);
 }

 /**
 * Asserts that two variables have the same type and value.
 * Used on objects, it asserts that two variables reference
 * the same object.
 *
 * @param mixed  $expected
 * @param mixed  $actual
 * @param string $message
 */
 public function assertSame(mixed $expected, mixed $actual, string $message = ''): void {
   $this->_testCaseShim->assertSame($expected, $actual, $message);
 }

 /**
 * Asserts that a file exists.
 *
 * @param string $filename
 * @param string $message
 *
 * @since Method available since Release 3.0.0
 */
  public function assertFileExists(string $filename, string $message = ''): void {
    $this->_testCaseShim->assertFileExists($filename, $message);
  }

  /**
 * Returns the number of assertions performed by this test.
 *
 * @return int
 *
 * @since Method available since Release 3.3.0
 */
 public function getNumAssertions(): int {
   return $this->_testCaseShim->getNumAssertions();
 }

 /**
  * @param array $groups
  *
  * @since Method available since Release 5.4.0
  */
 public function setGroups(mixed $groups): void {
   if ( is_array($groups) ) {
     $this->_testCaseShim->setGroups($groups);
   }
 }

 /**
  * Returns the size of the test.
  *
  * @return int
  *
  * @since Method available since Release 3.6.0
  */
 public function getSize(): int {
   return $this->_testCaseShim->getSize();
 }

 /**
  * Runs the bare test sequence.
  */
 public function runBare(): void {
   $this->_testCaseShim->runBare();
 }

 /**
  * Adds a value to the assertion counter.
  *
  * @param int $count
  *
  * @since Method available since Release 3.3.3
  */
 public function addToAssertionCount(int $count): void {
   $this->_testCaseShim->addToAssertionCount($count);
 }

 /**
  * Sets the name of a TestCase.
  *
  * @param  string
  */
 public function setName(string $name): void {
   $this->_testCaseShim->setName($name);
 }

 /**
  * Gets the name of a TestCase.
  *
  * @param bool $withDataSet
  *
  * @return string
  */
 public function getName(bool $withDataSet = true): string {
   return $this->_testCaseShim->getName($withDataSet);
 }

 /**
  * Sets the dependencies of a TestCase.
  *
  * @param array $dependencies
  *
  * @since Method available since Release 3.4.0
  */
 public function setDependencies(mixed $dependencies): void {
   if( is_array($dependencies) ) {
     $this->_testCaseShim->setDependencies($dependencies);
   }
 }

 /**
  * @param bool $beStrictAboutChangesToGlobalState
  *
  * @since Method available since Release 4.6.0
  */
 public function setBeStrictAboutChangesToGlobalState(mixed $strict): void {
   if ( is_bool($strict) ) {
     $this->_testCaseShim->setBeStrictAboutChangesToGlobalState($strict);
  }
 }

 /**
  * Calling this method in setUp() has no effect!
  *
  * @param bool $backupGlobals
  *
  * @since Method available since Release 3.3.0
  */
 public function setBackupGlobals(mixed $backupGlobals): void {
   if ( is_bool($backupGlobals) ) {
     $this->_testCaseShim->setBackupGlobals($backupGlobals);
   }
 }

 /**
  * Calling this method in setUp() has no effect!
  *
  * @param bool $backupStaticAttributes
  *
  * @since Method available since Release 3.4.0
  */
 public function setBackupStaticAttributes(mixed $backupStaticAttributes): void {
   if ( is_bool($backupStaticAttributes) ) {
     $this->_testCaseShim->setBackupStaticAttributes($backupStaticAttributes);
   }
 }

 /**
  * @param bool $runTestInSeparateProcess
  *
  * @throws PHPUnit_Framework_Exception
  *
  * @since Method available since Release 3.4.0
  */
 public function setRunTestInSeparateProcess(mixed $runTestInSeparateProcess): void {
   if ( is_bool($runTestInSeparateProcess) ) {
     $this->_testCaseShim->setRunTestInSeparateProcess($runTestInSeparateProcess);
   }
 }

 /**
  * @return bool
  *
  * @since Method available since Release 3.6.0
  */
 public function hasOutput(): bool {
   return $this->_testCaseShim->hasOutput();
 }

 /**
  * @return string
  *
  * @since Method available since Release 3.6.0
  */
 public function getActualOutput(): string {
   return $this->_testCaseShim->getActualOutput();
 }

 /**
  * Returns the status of this test.
  *
  * @return int
  *
  * @since Method available since Release 3.1.0
  */
 public function getStatus(): int {
   return $this->_testCaseShim->getStatus();
 }

 /**
 * @return PHPUnit_Framework_TestResult
 *
 * @since Method available since Release 3.5.7
 */
 public function getTestResultObject(): ?PHPUnit_Framework_TestResult {
   return $this->_testCaseShim->getTestResultObject();
 }

 /**
  * @return mixed
  *
  * @since Method available since Release 3.4.0
  */
 public function getResult(): mixed {
   return $this->_testCaseShim->getResult();
 }

 // @codeCoverageIgnoreEnd

}
