<?hh // strict

namespace Zynga\Framework\IO\Web\V1\Curl;

class MockedCurlRequest implements CurlInterface {
  
  private bool $setOptionsReturn = false;
  private mixed $curlExecReturn;
  private mixed $curlInfoReturn;
  
  public function __construct(bool $setOptionsReturn, mixed $curlExecReturn, mixed $curlInfoReturn) {
    $this->setOptionsReturn = $setOptionsReturn;
    $this->curlExecReturn = $curlExecReturn;
    $this->curlInfoReturn = $curlInfoReturn;
  }
  
  public function setOptionsArray(array<int, mixed> $options): bool {
    return $this->setOptionsReturn;
  }

  public function execute(): mixed {
    return $this->curlExecReturn;
  }

  public function getInfo(int $optName): mixed {
    return $this->curlInfoReturn;
  }

  public function close(): bool {
    // noop
    return true;
  }
}
