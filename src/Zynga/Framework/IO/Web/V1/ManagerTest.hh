<?hh // strict

namespace Zynga\Framework\IO\Web\V1;

use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Type\V1\UrlBox;

class ManagerTest extends TestCase {
  
  public function testMakeCurlRequestWithMock(): void {
    Manager::$useMockCurl = true;
    Manager::$useMockCurlResult = array('success' => true);
    $url = new UrlBox();
    $url->set('https://www.zynga.com');
    $postParams = Map{};
    $result = Manager::makeCurlRequest($url, $postParams);
    $this->assertEquals(1, count($result));
    $this->assertTrue(array_key_exists('success', $result));
    $this->assertTrue(($result['success'] == true));
  }

  public function testMakeCurlRequestWithoutMock(): void {
    Manager::$useMockCurl = false;
    $url = new UrlBox();
    $url->set('https://www.zynga.com');
    $postParams = Map{};
    $result = Manager::makeCurlRequest($url, $postParams);
    $this->assertEquals(0, count($result));
  }
}
