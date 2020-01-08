<?hh // strict

namespace Zynga\Framework\Service\V2\Client;

/**
 * A representation of the curl settings that can be applied to a curl handle
 */
class CurlOpts {
  public ?int $timeout;

  public function applyOptionsToHandle(resource $curlHandle): void {
    if ($this->timeout !== null) {
      curl_setopt($curlHandle, CURLOPT_TIMEOUT, $this->timeout);
    }
  }
}
