<?hh // strict

namespace Zynga\Framework\Service\V2;

use \ReflectionClass;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Service\V2\Client\BatchContainer;
use Zynga\Framework\Service\V2\Client\CurlOpts;
use
  Zynga\Framework\Service\V2\Exceptions\InvalidServiceResponse as InvalidServiceResponseException
;
use Zynga\Framework\Service\V2\Interfaces\ClientInterface;
use Zynga\Framework\Service\V2\Interfaces\RequestInterface;
use Zynga\Framework\Service\V2\Interfaces\ResponseInterface;
use Zynga\Framework\Service\V2\Response\Mock\Base as MockResponse;
use Zynga\Framework\StorableObject\V1\Interfaces\StorableObjectInterface;

class Client implements ClientInterface {
  private string $_serviceUrl;
  private classname<ResponseInterface> $_serviceResponseObject;
  private BatchContainer $_batches;
  private int $_requestCount;
  private bool $mockEnabled; // TODO: https://jira.corp.zynga.com/browse/POK-33682
  private MockResponse $mockResponse;

  public function __construct() {
    $this->_serviceUrl = '';
    $this->_serviceResponseObject = ResponseInterface::class;
    $this->_batches = new BatchContainer();
    $this->_requestCount = 0;
    $this->mockEnabled = false;
    $this->mockResponse = new MockResponse(true, 200);
  }

  public function getServiceUrl(): string {
    return $this->_serviceUrl;
  }

  public function setServiceUrl(string $serviceUrl): bool {
    $this->_serviceUrl = $serviceUrl;
    return true;
  }

  public function getServiceResponseObject(): classname<ResponseInterface> {
    return $this->_serviceResponseObject;
  }

  public function setServiceResponseObject(
    classname<ResponseInterface> $serviceResponseObject,
  ): bool {
    $this->_serviceResponseObject = $serviceResponseObject;
    return true;
  }

  public function getBatchLimit(): int {
    return $this->_batches->getBatchLimit();
  }

  public function setBatchLimit(int $batchLimit): bool {
    return $this->_batches->setBatchLimit($batchLimit);
  }

  public function clear(): bool {
    $this->_batches->clear();
    // --
    // JEO: We don't want to reset the request count, as we are trying to see
    // if there is a memory leak with number of requests sent.
    // --
    // $this->_requestCount = 0;
    return true;
  }

  public function getBatchCount(): int {
    return $this->_batches->getBatchCount();
  }

  public function getExcutableBatches(): Map<int, bool> {
    return $this->_batches->getExcutableBatches();
  }

  public function executeBatch(
    int $batchId,
    ?CurlOpts $curlOpts = null,
  ): bool {
    if ($this->mockEnabled) {
      return true;
    }

    try {
      return $this->_batches->executeBatch($batchId, $curlOpts);
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function getBatchResults(int $batchId): bool {
    if ($this->mockEnabled) {
      return true;
    }

    try {
      return $this->_batches->getBatchResults($batchId);
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function isBatchStillRunning(int $batchId): bool {
    if ($this->mockEnabled) {
      return false;
    }

    try {
      return $this->_batches->isBatchStillRunning($batchId);
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function addRequest(RequestInterface $object): (int, int) {
    $reflectionClass = new ReflectionClass($this->_serviceResponseObject);
    if (!$reflectionClass->isInstantiable()) {
      throw new InvalidServiceResponseException(
        "Adding request using bad _serviceResponseObject=".
        $this->_serviceResponseObject,
      );
    }

    $this->_requestCount++;
    return
      $this->_batches->addRequest(
        $object,
        $this->_serviceUrl,
        $this->_serviceResponseObject,
      );
  }

  public function getRequestResult(
    int $batchId,
    int $requestId,
  ): ResponseInterface {
    if ($this->mockEnabled) {
      return $this->mockResponse;
    }

    try {
      return $this->_batches->getRequestResult($batchId, $requestId);
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function wasRequestSuccessful(int $batchId, int $requestId): bool {
    if ($this->mockEnabled) {
      return true;
    }

    try {
      return $this->_batches->wasRequestSuccessful($batchId, $requestId);
    } catch (Exception $e) {
      throw $e;
    }
  }

  public function getRequestCount(): int {
    return $this->_requestCount;
  }

  public function setEnableMock(bool $mockEnabled): bool {
    $this->mockEnabled = $mockEnabled;
    return true;
  }

  public function getEnableMock(): bool {
    return $this->mockEnabled;
  }

  public function setMockResponse(MockResponse $mockResponse): bool {
    $this->mockResponse = $mockResponse;
    return true;
  }

}
