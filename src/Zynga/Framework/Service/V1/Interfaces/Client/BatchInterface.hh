<?hh // strict

namespace Zynga\Framework\Service\V1\Interfaces\Client;

use Zynga\Framework\Service\V1\Client\CurlOpts;
use Zynga\Framework\Service\V1\Interfaces\RequestInterface;
use Zynga\Framework\Service\V1\Interfaces\ResponseInterface;
use
  Zynga\Framework\Service\V1\Interfaces\ResponseInterface as ServiceResponse
;

interface BatchInterface {
  public function getServiceUrl(): string;
  public function setServiceUrl(string $serviceUrl): bool;
  public function getServiceResponseObject(): string;
  public function setServiceResponseObject(
    classname<ServiceResponse> $serviceResponseObject,
  ): bool;
  public function wasRequestSuccessful(int $requestId): bool;
  public function getBatchSize(): int;
  public function addRequest(RequestInterface $object): int;
  public function getBatchResults(): bool;
  public function getResponse(int $requestId): ResponseInterface;
  public function isBatchStarted(): bool;
  public function execute(?CurlOpts $curlOpts): bool;
  public function isBatchStillRunning(): bool;
  public function getRequestIds(): Set<int>;
}
