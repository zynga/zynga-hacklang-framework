<?hh //strict

namespace Zynga\Framework\Service\V1\Client\Mock;

use Zynga\Framework\Service\V1\Client\Mock\Batch as MockBatch;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class BatchTest extends TestCase {
  public function testIsBatchStarted(): void {
    $batch = new MockBatch();
    $this->assertFalse($batch->isBatchStarted());
  }
}
