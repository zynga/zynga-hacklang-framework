<?hh // strict

namespace Zynga\Framework\IO\Web\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\UrlBox;

use Zynga\Framework\IO\Web\V1\Curl\CurlInterface;
use Zynga\Framework\IO\Web\V1\Curl\CurlRequest;
use Zynga\Framework\IO\Web\V1\Curl\MockedCurlRequest;
use Zynga\Framework\IO\Web\V1\Curl\CurlResponsePayload;

class ManagerTest extends TestCase {
  
  <<__Override>>
  public function tearDown(): void {
    parent::tearDown();
    Manager::$useMockCurl = false;
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
    Manager::$useMockCurl = true;
    Manager::$curlExecReturn = null;
    $curlRequest = Manager::getCurlRequest($url);
    $curlResponse = $curlRequest->execute();
    $this->assertFalse($curlResponse->success);
    $this->assertEquals(array(), $curlResponse->payload);
  }
  
  public function testCurlResponseOnMockCurlReturnsSuccess(): void {
    $curlRequest = $this->getCurlRequest(true);
    $curlResponse = $curlRequest->execute();
    $this->assertTrue($curlResponse->success);
    $this->assertEquals(true, $curlResponse->payload['success']);
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
    $this->assertTrue($curlRequest->setOptionsArray(
    array(
      CURLOPT_RETURNTRANSFER => true
    )));
  }
  
  public function testCurlResponseOnCurlReturnsSuccess(): void {
    $curlRequest = $this->getCurlRequest(false);
    $curlRequest->setOptionsArray(array(CURLOPT_RETURNTRANSFER => true));
    $curlResponse = $curlRequest->execute();
    $this->assertTrue($curlResponse->success);
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
    Manager::$useMockCurl = $useMock;
    if($useMock == true) {
      Manager::$setOptionsReturn = true;
      Manager::$curlExecReturn = new CurlResponsePayload(true, array('success' => true));
      Manager::$curlInfoReturn = 200;
    }
    
    $url = new UrlBox();
    $url->set('https://www.zynga.com');
    $curlRequest = Manager::getCurlRequest($url);
    return $curlRequest;
  }
}
