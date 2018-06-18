<?hh //strict

namespace Zynga\Framework\Service\V2\Client;

use Zynga\Framework\Service\V2\Client;
use Zynga\Framework\Service\V2\Client\BatchContainer;
use Zynga\Framework\Service\V2\Client\Mock\Batch as MockBatch;
use Zynga\Framework\Service\V2\Client\Mock\BatchIsStartedBatch;
use Zynga\Framework\Service\V2\Client\Mock\FailedBatch;
use Zynga\Framework\Service\V2\Client\Mock\BatchContainer as MockBatchContainer;
use Zynga\Framework\Service\V2\Exceptions\BatchAlreadyStartedException;
use Zynga\Framework\Service\V2\Exceptions\BatchFailed as BatchFailedException;
use Zynga\Framework\Service\V2\Exceptions\BatchNotStartedException;
use Zynga\Framework\Service\V2\Test\RequestNoRequired;
use Zynga\Framework\Testing\TestCase\V2\Base as TestCase;
use Zynga\Framework\Service\V2\Test\Router\Services\Mock\V1\Response as MockResponse;

class BatchContainerTest extends TestCase {
  public function test_validSimpleUseCase(): void {
    // --
    // Kinda long test here, but we're testing for initial stand up and splitting
    // of number of items across multiple batches when batch limit is hit.
    // --
    $client = new Client();
    $client->setServiceUrl('http://localhost/');
    $client->setServiceResponseObject(
      MockResponse::class
    );
    $this->assertTrue($client->setBatchLimit(2));
    $this->assertEquals(2, $client->getBatchLimit());

    $this->assertTrue($client instanceof Client);

    $obj = new RequestNoRequired();

    $execBatches = $client->getExcutableBatches();
    $this->assertEquals(0, $execBatches->count());

    // add a single item to the batch
    list($batchId, $requestId) = $client->addRequest($obj);
    $this->assertEquals(1, $batchId);
    $this->assertEquals(1, $requestId);
    $this->assertEquals(1, $client->getBatchCount());

    // add another item to the batch, should be the same id as the first item
    list($sameBatchId, $nextRequestId) = $client->addRequest($obj);
    $this->assertEquals($batchId, $sameBatchId);
    $this->assertEquals(2, $nextRequestId);
    $this->assertEquals(1, $client->getBatchCount());

    // add another item to the client, should result in a new batch created
    list($nextBatchId, $nextRequestId) = $client->addRequest($obj);
    $this->assertEquals(2, $nextBatchId);
    $this->assertEquals(1, $nextRequestId);
    $this->assertEquals(2, $client->getBatchCount());

    $execBatches = $client->getExcutableBatches();
    $this->assertEquals(2, $execBatches->count());

    $this->assertTrue($client->clear());
    $this->assertEquals(0, $client->getBatchCount());
  }

  /**
   * @expectedException Zynga\Framework\Service\V2\Exceptions\UnknownBatchException
   */
  public function test_wasRequestSuccessful_unknownBatch(): void {
    $batchId = 1;
    $requestId = 1;

    $client = new Client();
    $client->wasRequestSuccessful($batchId, $requestId);
  }

  /**
   * @expectedException Zynga\Framework\Service\V2\Exceptions\UnknownBatchException
   */
  public function test_getBatchResults_unknownBatch(): void {
    $batchId = 1;
    $requestId = 1;

    $client = new Client();
    $client->getBatchResults($batchId);
  }

  /**
   * @expectedException Zynga\Framework\Service\V2\Exceptions\UnknownBatchException
   */
  public function test_getRequestResult_unknownBatch(): void {
    $batchId = 1;
    $requestId = 1;

    $client = new Client();
    $client->getRequestResult($batchId, $requestId);
  }

  /**
   * @expectedException Zynga\Framework\Service\V2\Exceptions\UnknownBatchException
   */
  public function test_executeBatch_unknownBatch(): void {
    $batchId = 1;
    $requestId = 1;

    $client = new Client();
    $client->executeBatch($batchId);
  }

  /**
   * @expectedException Zynga\Framework\Service\V2\Exceptions\UnknownBatchException
   */
  public function test_isBatchStillRunning_unknownBatch(): void {
    $batchId = 1;
    $requestId = 1;

    $client = new Client();
    $client->isBatchStillRunning($batchId);
  }

  public function testWasRequestSuccessfulBatchNotStartedException(): void {
    $batchContainer = new MockBatchContainer(new MockBatch());
    $this->expectException(BatchNotStartedException::class);
    $batchContainer->wasRequestSuccessful(0, 0);
  }

  public function testWasRequestSuccessful(): void {
    $batchContainer =
      new MockBatchContainer(new BatchIsStartedBatch(new MockBatch()));
    $result = $batchContainer->wasRequestSuccessful(0, 0);
    $this->assertFalse($result);
  }

  public function testWasRequestNotSuccessful(): void {
    $batchContainer =
      new MockBatchContainer(new FailedBatch(new MockBatch()));
    $batchContainer->addBatchToStack(0, new FailedBatch(new MockBatch()));
    $this->expectException(BatchFailedException::class);
    $result = $batchContainer->getRequestResult(0, 0);
  }

  public function testGetBatch(): void {
    $batchContainer = new BatchContainer();
    $batchContainer->addBatchToStack(0, new MockBatch());
    $this->assertTrue($batchContainer->batchIdExists(0));
    $batchContainer->getBatch(0);
  }

  public function testGetBatchResultsBatchNotStartedException(): void {
    $batchContainer = new MockBatchContainer(new MockBatch());
    $this->expectException(BatchNotStartedException::class);
    $batchContainer->getBatchResults(0);
  }

  public function testGetBatchResults(): void {
    $batchContainer =
      new MockBatchContainer(new BatchIsStartedBatch(new MockBatch()));
    $this->assertTrue($batchContainer->getBatchResults(0));
  }

  public function testGetRequestResultBatchNotStartedException(): void {
    $batchContainer = new MockBatchContainer(new MockBatch());
    $this->expectException(BatchNotStartedException::class);
    $batchContainer->getRequestResult(0, 0);
  }

  public function testGetRequestResultBatch(): void {
    $batchContainer =
      new MockBatchContainer(new BatchIsStartedBatch(new MockBatch()));
    $this->assertNotNull($batchContainer->getRequestResult(0, 0));
  }

  public function testExecuteBatchBatchAlreadyStartedException(): void {
    $batchContainer =
      new MockBatchContainer(new BatchIsStartedBatch(new MockBatch()));
    $this->expectException(BatchAlreadyStartedException::class);
    $batchContainer->executeBatch(0);
  }

  public function testIsBatchStillRunningBatchNotStartedException(): void {
    $batchContainer = new MockBatchContainer(new MockBatch());
    $this->expectException(BatchNotStartedException::class);
    $batchContainer->isBatchStillRunning(0);
  }
}
