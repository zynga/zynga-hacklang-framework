<?hh // strict

namespace Zynga\Framework\Service\V2\Client\Mock;

use Zynga\Framework\Service\V2\Client\Mock\Batch as MockBatch;
use Zynga\Framework\Service\V2\Interfaces\ResponseInterface;

class FailedBatch extends MockBatch {

  private MockBatch $baseMockBatch;

  public function __construct(MockBatch $baseMockBatch) {
    parent::__construct();
    $this->baseMockBatch = $baseMockBatch;
  }

  <<__Override>>
  public function isBatchStarted(): bool {
    return true;
  }

  <<__Override>>
  public function getBatchResults(): bool {
    return false;
  }
}
