<?hh // strict

namespace Zynga\Framework\Testing\TestCase\V2;

use Zynga\Framework\Testing\TestCase\V2\Base as ZyngaTestCase;
use Zynga\Framework\Testing\TestCase\V2\Config\Manager as TestManager;

use Zynga\Framework\Environment\TemporaryDirectory\V1\TemporaryDirectory;
use Zynga\Framework\Performance\V1\Tracker as PerformanceTracker;

use Zynga\Framework\Exception\V1\Exception;
use
  Zynga\Framework\Testing\TestCase\V2\Test\Mock\BrokenDatabaseFactoryResultSetReset
;
use Zynga\Framework\Testing\TestCase\V2\Test\Mock\BrokenDoHideLogs;
use Zynga\Framework\Testing\TestCase\V2\Test\Mock\BrokenEnableMocks;
use Zynga\Framework\Testing\TestCase\V2\Test\Mock\NotEnabled;
use Zynga\Framework\Testing\TestCase\V2\Test\Mock\RiggedGetStatus;
use Zynga\Framework\Testing\TestCase\V2\Test\Mock\TestCaseTestExample;
use Zynga\Framework\Testing\TestCase\V2\Test\Mock\TestCaseTestNoImplement;
use Zynga\PHPUnit\V2\Exceptions\AssertionFailedException;
use Zynga\PHPUnit\V2\Exceptions\TestError\SkippedException;
use Zynga\Framework\Testing\TestCase\V2\Test\Mock\TestCaseMock;

use \Exception as RawException;

class BaseTest extends ZyngaTestCase {

  public function test_perfFunction(): void {

    // test once uncached.
    $perf = $this->perf();
    $this->assertTrue($perf instanceof PerformanceTracker);

    // should be cached on second pass.
    $perf = $this->perf();
    $this->assertTrue($perf instanceof PerformanceTracker);

  }

  public function test_assertPerf(): void {
    $this->perf()->startTimer('testTimer');
    sleep(2);
    $this->perf()->endTimer('testTimer');
    $this->assertionFailureExpected();
    $this->assertPerf('testTimer', 1);
  }

  public function test_expectOutput(): void {
    echo "some-content";
    $this->expectOutput('some-content');
  }

  public function test_expectOutputPregMatch(): void {
    echo "some-content";
    $this->expectOutputPregMatch('/some/');
  }

  /**
   * Tests for a basic test case to instanciate with no issues.
   */
  public function testValidStandup(): void {

    try {
      $this->assertInstanceOf(ZyngaTestCase::class, $this);
      return;
    } catch (Exception $e) {
      throw $e;
    }

    $this->fail();

  }

  public function testInvalidObjectForClassImplements(): void {
    $this->assertionFailureExpected();
    $this->assertClassImplements('not-a-real-object', null);
  }

  /**
   * testValidAssertClassImplements
   */
  public function testValidAssertClassImplements(): void {

    $obj = new TestCaseTestExample();

    $this->assertClassImplements(
      'Zynga\\Framework\Testing\TestCase\V2\Test\Mock\TestCaseTestInterface',
      $obj,
    );

    $this->assertEquals(0, $this->getNumAssertions());

  }

  public function testAssertNotSameReturnsTrueOnDifferentValues(): void {
    $this->assertNotSame(1, 2);
  }

  /**
   * testInvalidAssertClassImplements
   */
  public function testInvalidAssertClassImplements(): void {

    $obj = new TestCaseTestExample();
    $this->assertionFailureExpected();
    $this->assertClassImplements('Presedente', $obj);

  }

  public function testInvalidClassAssertClassImplements(): void {
    $nonObjects = array();
    $nonObjects[] = 1;
    $nonObjects[] = null;
    $nonObjects[] = 'something';
    foreach ($nonObjects as $nonObject) {
      $this->assertionFailureExpected();
      $this->assertClassImplements(
        'Zynga\\TestCaseTest_Interface',
        $nonObjects,
      );
    }
  }

  public function testNonImplementation(): void {
    $obj = new TestCaseTestNoImplement();
    $this->assertionFailureExpected();
    $this->assertClassImplements(
      'Zynga\Framework\Testing\TestCase\V2\Test\Mock\TestCaseTestInterface',
      $obj,
    );
  }

  public function testDisabledTest(): void {

    $obj = new TestCaseTestExample();

    $this->assertClassImplements(
      'Zynga\Framework\Testing\TestCase\V2\Test\Mock\TestCaseTestInterface',
      $obj,
    );
    $this->assertEquals(0, $this->getNumAssertions());

  }

  public function runDisabledTest(): void {

    $mock = new TestCaseMock('TestCaseMock');

    $this->assertEquals(TestCaseMock::class, $mock->getTestCaseContext());

    $this->assertTrue($mock->isEnabled());

    TestManager::disableTest(
      $mock->getTestCaseContext(),
      __LINE__.' unit test',
    );

    $this->assertFalse($mock->isEnabled());

    TestManager::enableTest($mock->getTestCaseContext());

    $this->assertTrue($mock->isEnabled());

  }

  public function testBrokenDoHideLogs(): void {
    $test = new BrokenDoHideLogs('BrokenDoHideLogs');
    $this->assertTrue($test->doSetUpBeforeClass());
    $this->assertTrue($test->doTearDownAfterClass());
  }

  public function testBrokenEnableMocks(): void {
    $test = new BrokenEnableMocks('BrokenEnableMocks');
    $this->assertTrue($test->doSetUpBeforeClass());
    $this->assertTrue($test->doTearDownAfterClass());
  }

  public function testBrokenDatabaseFactoryResultSetReset(): void {
    $test = new BrokenDatabaseFactoryResultSetReset('BrokenEnableMocks');
    $this->assertTrue($test->doSetUpBeforeClass());
    $this->assertTrue($test->doTearDownAfterClass());
  }

  public function testNotEnabled(): void {
    $test = new NotEnabled('NotEnabled');

    $this->assertFalse($test->isEnabled());

    $this->expectException(SkippedException::class);

    $test->setUp();

  }

  public function testPerformanceProfiler(): void {
    $mock = new TestCaseMock('TestCaseMock');
    $mock->startProfiling();
    $foo = 0;
    for ($i = 0; $i < 100; $i++) {
      $this->assertEquals($i, $i);
      $foo++;
    }
    $mock->stopProfiling();
  }

  public function testAssertCount(): void {
    $mock = new TestCaseMock('TestCaseMock');

    $this->expectException(AssertionFailedException::class);
    $mock->assertCount(3, array(1, 3));

  }

  public function testToString(): void {

    $mock = new TestCaseMock('TestCaseMock');
    $mock->assertEquals(
      'Zynga\Framework\Testing\TestCase\V2\TestCaseMock::TestCaseMock',
      $mock->toString(),
    );

  }

  public function testCount(): void {

    $mock = new TestCaseMock('TestCaseMock');
    $mock->assertEquals(1, $mock->count());

  }

  public function testFail(): void {

    $mock = new TestCaseMock('TestCaseMock');
    $this->expectException(AssertionFailedException::class);
    $mock->fail();

  }

  public function testAssertNotFalse(): void {

    $mock = new TestCaseMock('TestCaseMock');
    $this->expectException(AssertionFailedException::class);
    $mock->assertNotFalse(false);

  }

  public function testSetGroups(): void {

    $testGroups = array('how', 'now');

    $mock = new TestCaseMock('TestCaseMock');
    $mock->setGroups($testGroups);
    $mock->assertEquals($testGroups, $mock->getGroups());

  }

  public function testSetName(): void {

    $testName = 'MsPeel';

    $mock = new TestCaseMock('TestCaseMock');
    $mock->setName($testName);
    $mock->assertEquals($testName, $mock->getName());

  }

  public function testDependencies(): void {

    $deps = Vector {'dep1', 'dep2'};

    $mock = new TestCaseMock('TestCaseMock');
    $mock->setDependencies($deps);
    $this->assertTrue($mock->hasDependencies());

  }

  public function testOutput(): void {
    $mock = new TestCaseMock('TestCaseMock');
    $this->assertFalse($mock->hasOutput());
    $this->assertEquals('', $mock->getActualOutput());
  }

  public function testGetStatus(): void {
    $mock = new TestCaseMock('TestCaseMock');
    $this->assertEquals(-1, $mock->getStatus());
  }

  // --
  // TODO: There is a ob close issue with this as phpunit is being a jerk and closing for us.
  // --
  public function testGetStatusRigged(): void {
    $mock = new RiggedGetStatus('testOk');
    $this->assertFalse($mock->hideLogs());
    $mock->runBare();
    $this->assertEquals(0, $mock->getStatus());

  }

  public function testAssertStringStartsWith(): void {
    $mock = new TestCaseMock('TestCaseMock');
    $this->expectException(AssertionFailedException::class);
    $mock->assertStringStartsWith('boo', 'foo');
  }

  public function testGetTestResultObject(): void {
    $mock = new TestCaseMock('TestCaseMock');
    $this->assertEquals(null, $mock->getTestResultObject());
  }

  public function testAssertNotNull(): void {
    $mock = new TestCaseMock('TestCaseMock');
    $this->expectException(AssertionFailedException::class);
    $mock->assertNotNull(null);
  }

  public function testAssertNotEmpty(): void {
    $foo = null;
    $mock = new TestCaseMock('TestCaseMock');
    $this->expectException(AssertionFailedException::class);
    $mock->assertNotEmpty($foo);
  }

  public function testAssertEmpty(): void {
    $mock = new TestCaseMock('TestCaseMock');
    $this->expectException(AssertionFailedException::class);
    $mock->assertEmpty('i am not empty');
  }

  public function testAssertGreaterThan(): void {
    $mock = new TestCaseMock('TestCaseMock');
    $this->expectException(AssertionFailedException::class);
    $mock->assertGreaterThan(2, 1);
  }

  public function testAssertNull(): void {
    $mock = new TestCaseMock('TestCaseMock');
    $this->expectException(AssertionFailedException::class);
    $mock->assertNull('i am not null');
  }

  public function testAssertSame(): void {
    $mock = new TestCaseMock('TestCaseMock');
    $this->expectException(AssertionFailedException::class);
    $mock->assertSame(1, 2);
  }

  public function testAssertFileExists(): void {
    $mock = new TestCaseMock('TestCaseMock');
    $this->expectException(AssertionFailedException::class);
    $mock->assertFileExists(
      '/tmp/jeo-'.mt_rand().'-file-does-not-exist-'.mt_rand(),
    );
  }

}
