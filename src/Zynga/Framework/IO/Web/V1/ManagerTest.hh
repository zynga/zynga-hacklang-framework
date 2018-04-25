<?hh // strict

namespace Zynga\Framework\IO\Web\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\UrlBox;

use Zynga\Framework\IO\Web\V1\Curl\CurlInterface;
use Zynga\Framework\IO\Web\V1\Curl\CurlRequest;
use Zynga\Framework\IO\Web\V1\Curl\MockedCurlRequest;

class ManagerTest extends TestCase {
  
  <<__Override>>
  public function tearDown(): void {
    parent::tearDown();
    Manager::$useMockCurl = false;
  }
  
  public function testMockedCurlRequestClassObject(): void {
    $curlRequest = $this->getCurlRequest(true);
    $this->assertEquals(MockedCurlRequest::class, get_class($curlRequest));
  }
  
  public function testSettingOptionsOnMockedCurlRequest(): void {
    $curlRequest = $this->getCurlRequest(true);
    $this->assertTrue($curlRequest->setOptionsArray(array()));
  }
  
  public function testCurlResponseOnExecutingAMockedCurlRequest(): void {
    $curlRequest = $this->getCurlRequest(true);
    $curlExecReturn = $curlRequest->execute();
    invariant(is_array($curlExecReturn), 'CurlExecReturn is not an array');
    $this->assertTrue(array_key_exists('success', $curlExecReturn));
    $this->assertEquals(true, $curlExecReturn['success']);
  }
  
  public function testGettingInfoOnMockedCurlRequest(): void {
    $curlRequest = $this->getCurlRequest(true);
    $curlInfoReturn = $curlRequest->getInfo(CURLINFO_HTTP_CODE);
    $this->assertEquals(200, $curlInfoReturn);
  }
  
  public function testClosingAMockedCurlRequest(): void {
    $curlRequest = $this->getCurlRequest(true);
    $this->assertTrue($curlRequest->close());
  }
  
  public function testCurlRequestClassObject(): void {
    $curlRequest = $this->getCurlRequest(false);
    $this->assertEquals(CurlRequest::class, get_class($curlRequest));
  }
  
  public function testSettingOptionsOnCurlRequest(): void {
    $curlRequest = $this->getCurlRequest(false);
    $this->assertTrue($curlRequest->setOptionsArray(
    array(
      CURLOPT_RETURNTRANSFER => true
    )));
  }
  
  public function testCurlResponseOnExecutingACurlRequest(): void {
    $curlRequest = $this->getCurlRequest(false);
    $this->assertTrue(($curlRequest->execute() == true));
  }
  
  public function testGettingInfoOnCurlRequest(): void {
    $curlRequest = $this->getCurlRequest(false);
    $curlRequest->execute();
    $curlHttpCode = $curlRequest->getInfo(CURLINFO_HTTP_CODE);
    $this->assertEquals(200, $curlHttpCode);
  }
  
  public function testClosingACurlRequest(): void {
    $curlRequest = $this->getCurlRequest(false);
    $this->assertTrue($curlRequest->close());
  }
    
  private function getCurlRequest(bool $useMock): CurlInterface {
    Manager::$useMockCurl = $useMock;
    if($useMock == true) {
      Manager::$setOptionsReturn = true;
      Manager::$curlExecReturn = array('success' => true);
      Manager::$curlInfoReturn = 200;
    }
    
    $url = new UrlBox();
    $url->set('https://www.zynga.com');
    $curlRequest = Manager::getCurlRequest($url);
    return $curlRequest;
  }
}
