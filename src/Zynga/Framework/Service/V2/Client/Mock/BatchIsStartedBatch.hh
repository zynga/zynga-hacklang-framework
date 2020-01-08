<?hh // strict

namespace Zynga\Framework\Service\V2\Client\Mock;

use Zynga\Framework\Service\V2\Client\Mock\Batch as MockBatch;
use Zynga\Framework\Service\V2\Interfaces\ResponseInterface;

class BatchIsStartedBatch extends MockBatch {

  private MockBatch $baseMockBatch;

  public function __construct(MockBatch $baseMockBatch) {
    parent::__construct();
    $this->baseMockBatch = $baseMockBatch;
  }

  public function hasResponse(int $requestId): bool {
    return $this->baseMockBatch->hasResponse($requestId);
  }

  public function requestSent(int $requestId): bool {
    return $this->baseMockBatch->requestSent($requestId);
  }

  public function isBatchStarted(): bool {
    return true;
  }
}
