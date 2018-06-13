<?hh // strict

namespace Zynga\Framework\Service\V1\Client;

use Zynga\Framework\Service\V1\Client\Batch as ClientBatch;
use Zynga\Framework\Service\V1\Client\CurlOpts;
use Zynga\Framework\Service\V1\Exceptions\BatchAlreadyStartedException;
use
  Zynga\Framework\Service\V1\Exceptions\BatchFailed as BatchFailedException
;
use Zynga\Framework\Service\V1\Exceptions\BatchNotStartedException;
use Zynga\Framework\Service\V1\Exceptions\UnknownBatchException;
use Zynga\Framework\Service\V1\Interfaces\Client\BatchInterface;
use Zynga\Framework\Service\V1\Interfaces\RequestInterface;
use
  Zynga\Framework\Service\V1\Interfaces\ResponseInterface as ServiceResponseInterface
;

class BatchContainer {
  private int $_batchId;
  private Map<int, BatchInterface> $_batches;
  private int $_batchLimit;

  public function __construct() {
    $this->_batchId = 0;
    $this->_batches = Map {};
    $this->_batchLimit = 100;
  }

  public function getBatchLimit(): int {
    return $this->_batchLimit;
  }

  public function setBatchLimit(int $batchLimit): bool {
    $this->_batchLimit = $batchLimit;
    return true;
  }

  public function clear(): bool {
    $this->_batches->clear();
    $this->_batchId = 0;
    return true;
  }

  public function getExcutableBatches(): Map<int, bool> {
    $batches = Map {};
    foreach ($this->_batches->keys() as $batchId) {
      $batches[$batchId] = true;
    }

    return $batches;
  }

  public function getBatchCount(): int {
    return $this->_batches->keys()->count();
  }

  public function getBatchId(): int {
    return $this->_batchId;
  }

  public function incrementBatchId(): bool {
    $this->_batchId++;
    return true;
  }

  public function addBatchToStack(int $batchId, ClientBatch $batch): bool {
    $this->_batches[$batchId] = $batch;
    return true;
  }

  public function addBatch(
    string $serviceUrl,
    classname<ServiceResponseInterface> $serviceResponseObject,
  ): int {
    $clientBatch = new ClientBatch();
    $clientBatch->setServiceResponseObject($serviceResponseObject);
    $clientBatch->setServiceUrl($serviceUrl);

    $this->incrementBatchId();
    $batchId = $this->getBatchId();

    $this->addBatchToStack($batchId, $clientBatch);

    return $this->getBatchId();
  }

  public function getBatchIdForNewRequest(
    string $serviceUrl,
    classname<ServiceResponseInterface> $serviceResponseObject,
  ): int {
    $batchId = 0;

    // fetch or recreate the batch id that we are operating against.
    if ($this->_batchId === 0) {
      $batchId = $this->addBatch($serviceUrl, $serviceResponseObject);
    } else {
      $batchId = $this->getBatchId();
    }

    // if the batch has it the size cap, bump to the next batch.
    $batchSize = $this->_batches[$batchId]->getBatchSize();

    if ($batchSize == $this->_batchLimit) {
      $batchId = $this->addBatch($serviceUrl, $serviceResponseObject);
    }

    return $batchId;
  }

  public function addRequest(
    RequestInterface $object,
    string $serviceUrl,
    classname<ServiceResponseInterface> $serviceResponseObject,
  ): (int, int) {
    $batchId =
      $this->getBatchIdForNewRequest($serviceUrl, $serviceResponseObject);

    // add the request into the batch.
    $requestId = $this->_batches[$batchId]->addRequest($object);

    // return the batchid and request id identifiers
    return tuple($batchId, $requestId);
  }

  public function wasRequestSuccessful(int $batchId, int $requestId): bool {
    if (!$this->batchIdExists($batchId)) {
      throw new UnknownBatchException('id='.$batchId);
    }

    $batch = $this->getBatch($batchId);

    if ($batch->isBatchStarted() !== true) {
      throw new BatchNotStartedException('id='.$batchId);
    }

    return $batch->wasRequestSuccessful($requestId);
  }

  public function getBatch(int $batchId): BatchInterface {
    return $this->_batches[$batchId];
  }

  public function batchIdExists(int $batchId): bool {
    return $this->_batches->containsKey($batchId);
  }

  public function getBatchResults(int $batchId): bool {
    if (!$this->batchIdExists($batchId)) {
      throw new UnknownBatchException('id='.$batchId);
    }

    $batch = $this->getBatch($batchId);

    if ($batch->isBatchStarted() !== true) {
      throw new BatchNotStartedException('id='.$batchId);
    }

    return $batch->getBatchResults();
  }

  public function getRequestResult(
    int $batchId,
    int $requestId,
  ): ServiceResponseInterface {
    if (!$this->batchIdExists($batchId)) {
      throw new UnknownBatchException('id='.$batchId);
    }

    $batch = $this->getBatch($batchId);
    if ($batch->isBatchStarted() !== true) {
      throw new BatchNotStartedException('id='.$batchId);
    }

    $success = $batch->getBatchResults();
    if (!$success) {
      throw new BatchFailedException();
    }

    return $batch->getResponse($requestId);
  }

  public function executeBatch(
    int $batchId,
    ?CurlOpts $curlOpts = null,
  ): bool {
    if (!$this->batchIdExists($batchId)) {
      throw new UnknownBatchException('id='.$batchId);
    }

    $batch = $this->getBatch($batchId);
    if ($batch->isBatchStarted() === true) {
      throw new BatchAlreadyStartedException('id='.$batchId);
    }

    // @codeCoverageIgnoreStart
    return $batch->execute($curlOpts);
    // @codeCoverageIgnoreEnd
  }

  public function isBatchStillRunning(int $batchId): bool {
    if (!$this->batchIdExists($batchId)) {
      throw new UnknownBatchException('id='.$batchId);
    }

    $batch = $this->getBatch($batchId);

    if ($batch->isBatchStarted() !== true) {
      throw new BatchNotStartedException('id='.$batchId);
    }

    // @codeCoverageIgnoreStart
    return $batch->isBatchStillRunning();
    // @codeCoverageIgnoreEnd
  }
}
