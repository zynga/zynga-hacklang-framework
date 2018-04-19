<?hh //strict

namespace Zynga\Framework\IO\Web\V1;

use \ReflectionClass;
use
  Zynga\Framework\IO\Web\V1\Exception\FailedExecution as FailedExecutionException
;
use
  Zynga\Framework\IO\Web\V1\Exception\FailedInitialization as FailedInitializationException
;
use
  Zynga\Framework\IO\Web\V1\Exception\UnexpectedHttpCode as UnexpectedHttpCodeException
;
use Zynga\Framework\IO\Web\V1\Manager;
use Zynga\Framework\IO\Web\V1\Mock\ManagerWithFailedExecuteCurl;
use Zynga\Framework\IO\Web\V1\Mock\ManagerWithFailedInitializeCurl;
use Zynga\Framework\IO\Web\V1\Mock\ManagerWithUnexpectedHttpCode;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\UrlBox;

class ManagerTest extends TestCase {

  public function testFailedInitializationException(): void {
    $url = new UrlBox();
    $this->expectException(FailedInitializationException::class);
    ManagerWithFailedInitializeCurl::putFile($url, '', '');
  }

  public function testFailedToExecuteException(): void {
    $url = new UrlBox();
    $this->expectException(FailedExecutionException::class);
    ManagerWithFailedExecuteCurl::putFile($url, '', '');
  }

  public function testUnexpectedHttpCodeException(): void {
    $url = new UrlBox();
    $this->expectException(UnexpectedHttpCodeException::class);
    ManagerWithUnexpectedHttpCode::putFile($url, '', '');
  }

  public function testGetCurlInfoWithInvalidResourceReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Web\V1\Manager');
    $method = $class->getMethod('getCurlInfo');
    $method->setAccessible(true);
    $result = $method->invoke(null, null, 0);
    $this->assertFalse($result);
  }

  public function testGetCurlInfoWithValidResourceDoesNotReturnFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Web\V1\Manager');
    $method = $class->getMethod('getCurlInfo');
    $method->setAccessible(true);
    $resourceHandle = curl_init('');
    $result = $method->invoke(null, $resourceHandle, CURLINFO_HTTP_CODE);
    $this->assertFalse($result === false);
  }

  public function testExecuteWithInvalidResourceReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Web\V1\Manager');
    $method = $class->getMethod('executeCurl');
    $method->setAccessible(true);
    $result = $method->invoke(null, null);
    $this->assertFalse($result);
  }

  public function testExecuteWithValidResourceThatFailsReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Web\V1\Manager');
    $method = $class->getMethod('executeCurl');
    $method->setAccessible(true);
    $resourceHandle = curl_init('');
    $result = $method->invoke(null, $resourceHandle);
    $this->assertFalse($result);
  }

  public function testSetCurlOptionWithInvalidResourceReturnsFalse(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Web\V1\Manager');
    $method = $class->getMethod('setCurlOption');
    $method->setAccessible(true);
    $result = $method->invoke(null, null, 0, 0);
    $this->assertFalse($result);
  }

  public function testSetCurlOptionWithValidResourceReturnsTrue(): void {
    $class = new ReflectionClass('Zynga\Framework\IO\Web\V1\Manager');
    $method = $class->getMethod('setCurlOption');
    $method->setAccessible(true);
    $resourceHandle = curl_init('');
    $result =
      $method->invoke(null, $resourceHandle, CURLOPT_CUSTOMREQUEST, "PUT");
    $this->assertTrue($result);
  }

}
