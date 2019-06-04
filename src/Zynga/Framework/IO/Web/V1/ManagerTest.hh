<?hh // strict

namespace Zynga\Framework\IO\Web\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\UrlBox;

use Zynga\Framework\Environment\CodePath\V1\CodePath;
use Zynga\Framework\IO\Web\V1\Curl\CurlRequest;
use Zynga\Framework\IO\Web\V1\Exceptions\UnexpectedHttpCodeException;
use Zynga\Framework\IO\Web\V1\Exceptions\FailedExecutionException;
use Zynga\Framework\IO\Web\V1\Curl\CurlResponsePayload;
use Zynga\Framework\IO\Web\V1\Curl\MockedCurlRequest;
use Zynga\Framework\IO\Web\V1\Interfaces\CurlInterface;

class ManagerTest extends TestCase {

  <<__Override>>
  public function tearDown(): void {
    parent::tearDown();
    Manager::setUseMock(false);
    $tmpDir = $this->getTempTestDir();
    system('rm -rf '.escapeshellarg($tmpDir));
  }

  public function testMockCurlReturnsMockedCurlRequestClass(): void {
    $curlRequest = $this->getCurlRequest(true);
    $this->assertEquals(MockedCurlRequest::class, get_class($curlRequest));
  }

  public function testSettingOptionsOnMockCurlReturnsTrue(): void {
    $curlRequest = $this->getCurlRequest(true);
    $this->assertTrue($curlRequest->setOptionsArray(array()));
  }

  public function testCurlResponseOnMockCurlReturnsFailure(): void {
    $url = new UrlBox();
    $url->set('https://www.zynga.com');
    Manager::clear();
    Manager::setUseMock(true);
    $curlRequest = Manager::getCurlRequest($url);
    $curlResponse = $curlRequest->execute();
    $this->assertFalse($curlResponse->getSuccess());
    $this->assertEquals(array(), $curlResponse->getPayload());
  }

  public function testCurlResponseOnMockCurlReturnsSuccess(): void {
    $curlRequest = $this->getCurlRequest(true);
    $curlResponse = $curlRequest->execute();
    $this->assertTrue($curlResponse->getSuccess());
    $this->assertEquals(true, $curlResponse->getPayload()['success']);
  }

  public function testGettingHttpCodeOnMockCurlReturns200(): void {
    $curlRequest = $this->getCurlRequest(true);
    $curlInfoReturn = $curlRequest->getInfo(CURLINFO_HTTP_CODE);
    $this->assertEquals(200, $curlInfoReturn);
  }

  public function testClosingAMockCurlTrue(): void {
    $curlRequest = $this->getCurlRequest(true);
    $this->assertTrue($curlRequest->close());
  }

  public function testCurlReturnsCurlRequestClass(): void {
    $curlRequest = $this->getCurlRequest(false);
    $this->assertEquals(CurlRequest::class, get_class($curlRequest));
  }

  public function testSettingOptionsOnCurlReturnsTrue(): void {
    $curlRequest = $this->getCurlRequest(false);
    $this->assertTrue(
      $curlRequest->setOptionsArray(array(CURLOPT_RETURNTRANSFER => true)),
    );
  }

  public function testCurlResponseOnCurlReturnsSuccess(): void {
    $curlRequest = $this->getCurlRequest(false);
    $curlRequest->setOptionsArray(array(CURLOPT_RETURNTRANSFER => true));
    $curlResponse = $curlRequest->execute();
    $this->assertTrue($curlResponse->getSuccess());
  }

  public function testGettingHttpCodeOnCurlReturns200(): void {
    $curlRequest = $this->getCurlRequest(false);
    $curlRequest->execute();
    $curlHttpCode = $curlRequest->getInfo(CURLINFO_HTTP_CODE);
    $this->assertEquals(200, $curlHttpCode);
  }

  public function testClosingACurlReturnsTrue(): void {
    $curlRequest = $this->getCurlRequest(false);
    $this->assertTrue($curlRequest->close());
  }

  private function getCurlRequest(bool $useMock): CurlInterface {

    Manager::setUseMock($useMock);

    if ($useMock == true) {
      Manager::setOptionsReturn(true);
      Manager::setMockExecReturn(
        new CurlResponsePayload(true, array('success' => true)),
      );
      Manager::setMockInfoReturn(Map {CURLINFO_HTTP_CODE => 200});
    }

    $url = new UrlBox();
    $url->set('https://www.zynga.com');
    $curlRequest = Manager::getCurlRequest($url);
    return $curlRequest;
  }

  private function getTempTestDir(): string {
    return CodePath::getRoot().'/tmp/Web-IO-ManagerTest';
  }

  public function testRunPutFile(): void {

    $testDir = $this->getTempTestDir();
    if (!is_dir($testDir)) {
      mkdir($testDir, 0755, true);
    }

    $testFile = $testDir.'/gibberish-file-01.txt';
    if (!is_file($testFile)) {
      touch($testFile);
    }

    $url = new UrlBox();
    $url->set('https://postman-echo.com/put');

    $mimeType = 'text/plain';

    $this->assertTrue(Manager::putFile($url, $testFile, $mimeType));

  }

  public function testRunPutFileFails_HTTPCODE(): void {

    $testDir = $this->getTempTestDir();
    if (!is_dir($testDir)) {
      mkdir($testDir, 0755, true);
    }

    $testFile = $testDir.'/gibberish-file-02.txt';
    if (!is_file($testFile)) {
      touch($testFile);
    }

    $url = new UrlBox();
    $url->set('https://zynga.com');

    $mimeType = 'text/plain';

    $this->expectException(UnexpectedHttpCodeException::class);
    Manager::putFile($url, $testFile, $mimeType);

  }

  public function testRunPutFileFails_BadUrl(): void {

    $testDir = $this->getTempTestDir();
    if (!is_dir($testDir)) {
      mkdir($testDir, 0755, true);
    }

    $testFile = $testDir.'/gibberish-file-02.txt';
    if (!is_file($testFile)) {
      touch($testFile);
    }

    $url = new UrlBox();

    $mimeType = 'text/plain';

    $this->expectException(FailedExecutionException::class);
    Manager::putFile($url, $testFile, $mimeType);

  }

}
