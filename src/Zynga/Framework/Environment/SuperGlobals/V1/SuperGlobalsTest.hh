<?hh //strict

namespace Zynga\Framework\Environment\SuperGlobals\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Environment\SuperGlobals\V1\SuperGlobals;
use Zynga\Framework\Environment\CodePath\V1\CodePath;

class SuperGlobalsTest extends TestCase {
  public string $_scriptName;

  public function __construct(string $name) {

    parent::__construct($name);

    $this->_scriptName = '';

  }

  public function setUp(): void {
    $this->_scriptName = SuperGlobals::getScriptName();
  }

  public function tearDown(): void {
    SuperGlobals::setScriptName($this->_scriptName);
  }

  public function test_bodyFilePath(): void {
    $this->assertEquals(
      SuperGlobals::DEFAULT_HTTP_BODY_FILE,
      SuperGlobals::getBodyFilePath(),
    );
    $this->assertTrue(SuperGlobals::setBodyFilePath('/etc/passwd'));
    $this->assertEquals('/etc/passwd', SuperGlobals::getBodyFilePath());
    $this->assertTrue(
      SuperGlobals::setBodyFilePath(SuperGlobals::DEFAULT_HTTP_BODY_FILE),
    );
  }

  public function testValid(): void {
    $this->assertEquals(Map::class, get_class(SuperGlobals::getGetAsMap()));
    $this->assertEquals(Map::class, get_class(SuperGlobals::getPostAsMap()));
    $this->assertEquals(
      Map::class,
      get_class(SuperGlobals::getRequestAsMap()),
    );

    $key = 'someRandoKey';
    $value = mt_rand(1, 10000);

    $requestVars = SuperGlobals::getRequestAsMap();
    $this->assertFalse($requestVars->containsKey($key));

    $requestVars = null;

    $this->assertTrue(SuperGlobals::setRequestPairing($key, $value));

    $requestVars = SuperGlobals::getRequestAsMap();
    $this->assertTrue($requestVars->containsKey($key));
    $this->assertEquals($value, $requestVars->get($key));

    $requestVars = null;

    SuperGlobals::unsetRequestPairing($key);
    $requestVars = SuperGlobals::getRequestAsMap();
    $this->assertFalse($requestVars->containsKey($key));

    $serverVars = SuperGlobals::getServerAsMap();
    $this->assertTrue($serverVars instanceof Map);

  }

  public function testScriptNameNotFound(): void {
    $this->assertTrue(SuperGlobals::unsetScriptName());
    $this->assertEquals('', SuperGlobals::getScriptName());
  }

  public function testGetScriptCommandLineArguments(): void {

    $scriptArgs = SuperGlobals::getScriptCommandLineArguments();

    $this->assertTrue(is_array($scriptArgs));

    // clear the arguments
    $this->assertTrue(SuperGlobals::clearScriptCommandLineArguments());
    $this->assertEquals(null, SuperGlobals::getScriptCommandLineArguments());

    // reset the args
    if (is_array($scriptArgs)) {

      SuperGlobals::setScriptCommandLineArguments($scriptArgs);

      $newValue = SuperGlobals::getScriptCommandLineArguments();

      $this->assertEquals(count($scriptArgs), count($newValue));

    }

  }

  public function testGetScriptUri(): void {

    $currentUri = SuperGlobals::getScriptUri();

    SuperGlobals::clearScriptUri();
    $this->assertEquals('', SuperGlobals::getScriptUri());
    SuperGlobals::setScriptUri('http://www.cnn.com');
    $this->assertEquals('http://www.cnn.com', SuperGlobals::getScriptUri());
    $this->assertTrue(SuperGlobals::clearScriptUri());

    $this->assertTrue(SuperGlobals::setScriptUri($currentUri));

  }

  public function test_clearRequest(): void {
    $this->assertTrue(SuperGlobals::clearRequest());
  }

  public function findTestDir(): string {
    $testDir = dirname(__FILE__);
    $testDir = dirname($testDir);       // V1
    $testDir = dirname($testDir);       // SuperGlobals
    $testDir = dirname($testDir);       // Environment
    $testDir = dirname($testDir);       // Framework
    $testDir = dirname($testDir);       // Zynga
    $testDir = dirname($testDir);       // src
    return $testDir;
  }

  public function test_getRequestBodyAsMap_Empty(): void {

    $requestFile = $this->findTestDir() . '/tests/empty_request_body.js';

    SuperGlobals::setBodyFilePath($requestFile);

    $data = SuperGlobals::getRequestBodyAsMap();
    $this->assertEquals(0, $data->count());

  }

  public function test_getRequestBodyAsMap_SomeData(): void {

    $requestFile = $this->findTestDir().'/tests/some_data.js';

    SuperGlobals::setBodyFilePath($requestFile);

    $data = SuperGlobals::getRequestBodyAsMap();
    $this->assertEquals(3, $data->count());

  }

}
