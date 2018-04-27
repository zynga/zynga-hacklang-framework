<?hh // strict

namespace Zynga\Framework\IO\Web\V1\Curl;

use Zynga\Framework\Type\V1\UrlBox;

class CurlRequest implements CurlInterface {
  private resource $curlHandle;
  
  public function __construct(UrlBox $url) {
    $this->curlHandle = curl_init($url->get());
  }
  
  public function setOptionsArray(array<int, mixed> $options): bool {
    return curl_setopt_array($this->curlHandle, $options);
  }

  public function execute(): CurlResponsePayload {
    $curlResult = curl_exec($this->curlHandle);
    if(is_bool($curlResult)) {
      return new CurlResponsePayload($curlResult, array());
    }
    
    $curlResultArray = json_decode($curlResult, true);
    if($curlResultArray == null) {
      $curlResultArray = array();
    }
    return new CurlResponsePayload(true, $curlResultArray);
  }

  public function getInfo(int $optName): mixed {
    return curl_getinfo($this->curlHandle, $optName);
  }

  public function close(): bool {
    curl_close($this->curlHandle);
    return true;
  }
}
