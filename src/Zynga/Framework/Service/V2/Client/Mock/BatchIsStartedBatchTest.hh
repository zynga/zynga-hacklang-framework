<?hh //strict

namespace Zynga\Framework\Service\V2\Client\Mock;

use Zynga\Framework\Service\V2\Client\Mock\Batch as MockBatch;
use Zynga\Framework\Service\V2\Client\Mock\BatchIsStartedBatch as Batch;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;

class BatchIsStartedBatchTest extends TestCase {
  public function testHasResponse(): void {
    $batch = new Batch(new MockBatch());
    $this->assertFalse($batch->hasResponse(0));
  }

  public function testRequestSent(): void {
    $batch = new Batch(new MockBatch());
    $this->assertFalse($batch->requestSent(0));
  }
}
