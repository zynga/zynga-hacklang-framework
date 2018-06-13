<?hh // strict

namespace Zynga\Framework\Service\V1\Client;

use Zynga\Framework\Dynamic\V1\DynamicClassCreation;
use Zynga\Framework\Exception\V1\Exception;
use Zynga\Framework\Service\V1\Interfaces\Client\BatchInterface;
use Zynga\Framework\Service\V1\Interfaces\RequestInterface;
use
  Zynga\Framework\Service\V1\Interfaces\ResponseInterface as ServiceResponseInterface
;
use Zynga\Framework\StorableObject\V1\Exceptions\NoFieldsFoundException;

/**
 * A collection of microservice requests that can be asynchronously sent
 */
class Batch implements BatchInterface {
  private string $_serviceUrl;
  private classname<ServiceResponseInterface> $_serviceResponseObject;
  private int $_requestId;
  private Map<int, RequestInterface> $_requests;
  private Map<int, ServiceResponseInterface> $_responses;

  private ?resource $_curlMultiHandle;
  private Map<int, resource> $_curlHandles;

  public function __construct() {
    $this->_serviceUrl = '';
    $this->_serviceResponseObject = ServiceResponseInterface::class;
    $this->_requestId = 0;
    $this->_requests = Map {};
    $this->_responses = Map {};
    $this->_curlMultiHandle = null;
    $this->_curlHandles = Map {};
  }

  public function getRequestIds(): Set<int> {
    return new Set($this->_requests->keys());
  }

  public function getServiceUrl(): string {
    return $this->_serviceUrl;
  }

  public function setServiceUrl(string $serviceUrl): bool {
    $this->_serviceUrl = $serviceUrl;
    return true;
  }

  public function getServiceResponseObject(): string {
    return $this->_serviceResponseObject;
  }

  public function setServiceResponseObject(
    classname<ServiceResponseInterface> $serviceResponseObject,
  ): bool {
    $this->_serviceResponseObject = $serviceResponseObject;
    return true;
  }

  public function getRequestId(): int {
    return $this->_requestId;
  }

  public function wasRequestSuccessful(int $requestId): bool {
    if (!$this->requestSent($requestId)) {
      return false;
    }

    if ($this->hasResponse($requestId)) {
      $response = $this->getResponse($requestId);
      return $response->success()->get();
    }

    return false;
  }

  public function hasResponse(int $requestId): bool {
    return $this->_responses->containsKey($requestId);
  }

  public function requestSent(int $requestId): bool {
    return $this->_requests->containsKey($requestId);
  }

  public function getResponse(int $requestId): ServiceResponseInterface {
    if ($this->hasResponse($requestId) === true) {
      return $this->_responses[$requestId];
    }

    throw new Exception('No such response');
  }

  public function getBatchSize(): int {
    return $this->_requests->count();
  }

  public function isBatchStarted(): bool {
    if (is_resource($this->_curlMultiHandle) ||
        $this->_curlHandles->count() > 0) {
      return true;
    }

    return false;
  }

  public function addRequest(RequestInterface $object): int {
    if ($this->isBatchStarted() === true) {
      throw new Exception('BatchAlreadyExecuting');
    }

    // increment the request id.
    $this->_requestId++;

    // set the request into the map.
    $this->_requests->set($this->_requestId, $object);

    // return the request id.
    return $this->_requestId;
  }

  public function execute(?CurlOpts $curlOpts = null): bool {
    if ($this->isBatchStarted() === true) {
      throw new Exception('BatchAlreadyExecuting');
    }

    $this->_curlMultiHandle = curl_multi_init();
    foreach ($this->_requests as $requestId => $requestParams) {
      $body = '';
      try {
        $body = $requestParams->export()->asJSON();
      } catch (NoFieldsFoundException $e) {
        // thats okay, not all of our endpoints have parameters.
      }

      $curlHandle = curl_init();
      curl_setopt($curlHandle, CURLOPT_URL, $this->_serviceUrl);
      curl_setopt($curlHandle, CURLOPT_POST, true);
      curl_setopt($curlHandle, CURLOPT_POSTFIELDS, $body);
      curl_setopt($curlHandle, CURLOPT_RETURNTRANSFER, 1);
      if ($curlOpts !== null) {
        $curlOpts->applyOptionsToHandle($curlHandle);
      }

      curl_multi_add_handle($this->_curlMultiHandle, $curlHandle);
      $this->_curlHandles[$requestId] = $curlHandle;
    }

    $active = -1;
    do {
      $result = curl_multi_exec($this->_curlMultiHandle, $active);
    } while ($result === CURLM_CALL_MULTI_PERFORM ||
             ($active !== 0 && $result === CURLM_OK));

    return true;
  }

  public function isBatchStillRunning(): bool {
    if (is_null($this->_curlMultiHandle)) {
      return false;
    }

    $stillRunning = null;
    $curlMultiReturn =
      curl_multi_exec($this->_curlMultiHandle, $stillRunning);

    // --
    // stillRunning: When curl is in a happy state stillRunning > 0 will work
    //  as the value is the number of handles that are still actively pulling
    //  data.
    //
    // curlMultiReturn: We are looking for if curl is performing a operation or
    //  is still in a ok running state.
    //
    // --
    if ((!is_null($stillRunning) && $stillRunning > 0) &&
        ($curlMultiReturn === CURLM_CALL_MULTI_PERFORM ||
         $curlMultiReturn === CURLM_OK)) {
      return true;
    }

    return false;
  }

  public function getBatchResults(): bool {
    if ($this->isBatchStillRunning() === true) {
      return false;
    }

    if ($this->_curlHandles->count() == 0) {
      return true;
    }

    foreach ($this->_curlHandles as $requestId => $curlHandle) {
      $content = curl_multi_getcontent($curlHandle);

      // by default our content is json
      if ($content === null ||
          $this->parseJsonResults($requestId, $content) !== true) {
        // TODO: JEO need to pop the errors into the error stack.
      }

      // pull the handle off the multi batch, as we are done with it.
      curl_multi_remove_handle($this->_curlMultiHandle, $curlHandle);
    }

    curl_multi_close($this->_curlMultiHandle);
    return true;
  }

  public function parseJsonResults(
    int $requestId,
    string $content,
  ): (bool, string) {
    // first check to see if it's even a json payload
    if (strpos($content, '{') != 0) {
      // invalid json
      return tuple(false, 'missing json leader');
    }

    // check for the trail of json data.
    if (strpos($content, '}', strlen($content))) {
      return tuple(false, 'missing json tail');
    }

    try {

      $obj = DynamicClassCreation::createClassByName(
        $this->_serviceResponseObject,
        Vector {},
      );
      if (!$obj instanceof ServiceResponseInterface) {
        return tuple(false, 'not-a-response-object');
      }

      $obj->import()->fromJSON($content);
      $this->setResponse($requestId, $obj);
      return tuple(true, '');
    } catch (Exception $e) {
      // TODO: put these items into the error stack.
      return tuple(false, $e->getMessage());
    }
  }

  public function setResponse(
    int $requestId,
    ServiceResponseInterface $obj,
  ): bool {
    $this->_responses[$requestId] = $obj;
    return true;
  }
}
