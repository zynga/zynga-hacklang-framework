<?hh //strict

namespace Zynga\Framework\Service\V1\Client;

use Zynga\Framework\Service\V1\Client\CurlOpts;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class CurlOptsTest extends TestCase {

  public function testSetNothing(): void {
    $curlOpts = new CurlOpts();
    $curlHandle = curl_init();
    $curlOpts->applyOptionsToHandle($curlHandle);
    // curl actually has no way to verify an option was set or not,
    // so there's nothing to do here
    $this->assertTrue(true);
  }

  public function testSetTimestamp(): void {
    $curlOpts = new CurlOpts();
    $curlOpts->timeout = 20;
    $curlHandle = curl_init();
    $curlOpts->applyOptionsToHandle($curlHandle);
    // curl actually has no way to verify an option was set or not,
    // so there's nothing to do here
    $this->assertTrue(true);
  }
}
