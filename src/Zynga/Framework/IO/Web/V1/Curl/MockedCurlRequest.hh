<?hh // strict

namespace Zynga\Framework\IO\Web\V1\Curl;

class MockedCurlRequest implements CurlInterface {
  
  private bool $setOptionsReturn = false;
  private CurlResponsePayload $curlExecReturn;
  private Map<int, mixed> $curlInfoReturn;

  public function __construct(bool $setOptionsReturn, CurlResponsePayload $curlExecReturn, Map<int, mixed> $curlInfoReturn) {
    $this->setOptionsReturn = $setOptionsReturn;
    $this->curlExecReturn = $curlExecReturn;
    $this->curlInfoReturn = $curlInfoReturn;
  }
  
  public function setOptionsArray(array<int, mixed> $options): bool {
    return $this->setOptionsReturn;
  }

  public function execute(): CurlResponsePayload {
    return $this->curlExecReturn;
  }

  public function getInfo(int $optName): mixed {
    return $this->curlInfoReturn[$optName];
  }

  public function close(): bool {
    // noop
    return true;
  }
}
