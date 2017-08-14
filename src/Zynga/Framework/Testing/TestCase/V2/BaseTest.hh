<?hh // strict

namespace Zynga\Framework\Testing\TestCase\V2;

use Zynga\Framework\Testing\TestCase\V2\Base as ZyngaTestCase;
use Zynga\Framework\Testing\TestCase\V2\Config\Manager as TestManager;

use Zynga\Framework\Environment\TemporaryDirectory\V1\TemporaryDirectory;
use Zynga\Framework\Performance\V1\Tracker as PerformanceTracker;

use Zynga\Framework\Exception\V1\Exception;

// mock interface for testing
interface TestCaseTest_Interface {}

// mock class for testing
class TestCaseTest_Example implements TestCaseTest_Interface {}

class TestCaseTestNoImplement {}

class TestCaseMock extends ZyngaTestCase {}

class TestCaseTest extends ZyngaTestCase {

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
    $this->expectOutputPregMatch('/some/', ob_get_contents());
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
   * Testing the noop built in test.
   */
  public function testBaseNoop(): bool {
    $rv = $this->testNoop();
    $this->assertTrue($rv);
    return true;
  }

  /**
   * testValidAssertClassImplements
   */
  public function testValidAssertClassImplements(): void {

    $obj = new TestCaseTest_Example();

    $this->assertClassImplements('Zynga\\Framework\Testing\TestCase\V2\TestCaseTest_Interface', $obj);

    $this->assertEquals(0, $this->getNumAssertions());

  }

  /**
   * testInvalidAssertClassImplements
   */
  public function testInvalidAssertClassImplements(): void {

    $obj = new TestCaseTest_Example();
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
    $this->assertClassImplements('Zynga\Framework\Testing\TestCase\V2\TestCaseTest_Interface', $obj);
  }

  public function testDisabledTest(): void {

    $obj = new TestCaseTest_Example();

    $this->assertClassImplements('Zynga\Framework\Testing\TestCase\V2\TestCaseTest_Interface', $obj);
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

}
