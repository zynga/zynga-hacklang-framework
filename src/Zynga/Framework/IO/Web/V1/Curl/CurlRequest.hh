<?hh // strict

namespace Zynga\Framework\IO\Web\V1\Curl;
use Zynga\Framework\Type\V1\UrlBox;

class CurlRequest implements CurlInterface {
  private mixed $curlHandle;
  
  public function __construct(UrlBox $url) {
    $this->curlHandle = curl_init($url->get());
  }
  
  public function setOptionsArray(array<int, mixed> $options): bool {
    return curl_setopt_array($this->curlHandle, $options);
  }

  public function execute(): mixed {
    return curl_exec($this->curlHandle);
  }

  public function getInfo(int $optName): mixed {
    return curl_getinfo($this->curlHandle, $optName);
  }

  public function close(): bool {
    curl_close($this->curlHandle);
    return true;
  }
}
