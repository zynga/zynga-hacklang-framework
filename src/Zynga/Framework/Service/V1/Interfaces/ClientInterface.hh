<?hh // strict

namespace Zynga\Framework\Service\V1\Interfaces;

use Zynga\Framework\Service\V1\Interfaces\ResponseInterface;

interface ClientInterface {
  public function getServiceUrl(): string;
  public function setServiceUrl(string $serviceUrl): bool;
  public function getServiceResponseObject(): classname<ResponseInterface>;
  public function setServiceResponseObject(
    classname<ResponseInterface> $serviceResponseObject,
  ): bool;
  public function getBatchLimit(): int;
  public function setBatchLimit(int $batchLimit): bool;
  public function clear(): bool;
  public function getBatchCount(): int;
  public function getExcutableBatches(): Map<int, bool>;
  public function executeBatch(int $batchId): bool;
  public function isBatchStillRunning(int $batchId): bool;
  public function addRequest(RequestInterface $object): (int, int);
  public function getRequestCount(): int;
}
