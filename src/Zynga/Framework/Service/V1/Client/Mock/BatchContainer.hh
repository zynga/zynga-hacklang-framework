<?hh // strict

namespace Zynga\Framework\Service\V1\Client\Mock;

use Zynga\Framework\Service\V1\Client\BatchContainer as BaseBatchContainer;
use Zynga\Framework\Service\V1\Interfaces\Client\BatchInterface;

class BatchContainer extends BaseBatchContainer {

  private BatchInterface $batch;

  public function __construct(BatchInterface $batch) {
    parent::__construct();
    $this->batch = $batch;
  }

  public function getBatch(int $batchId): BatchInterface {
    return $this->batch;
  }

  public function batchIdExists(int $batchId): bool {
    return true;
  }
}
